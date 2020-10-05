#include "windows-wmi_block.h"

WMIMonitorBlock::WMIMonitorBlock(const char* id,const char* name,const char* parameters) :
MonitorBlock(id,name,_block_type::collector,parameters,_output_type::ClearText) {};

WMIMonitorBlock::~WMIMonitorBlock() {};

bool WMIMonitorBlock::execute() {
    try {
        rapidjson::Document parsed_parameters = this->parse_parameters();
        const std::string& wmi_namespace = parsed_parameters["namespace"].GetString();
        const std::string& wmi_query = parsed_parameters["query"].GetString();
        const std::string& wmi_target = parsed_parameters["target"].GetString();
        
        if (FAILED(CoInitializeEx(0,COINIT_MULTITHREADED)))
        {
            throw std::runtime_error("Failed to initialize COM library.");
        }

        if (FAILED(CoInitializeSecurity(NULL,-1,NULL,NULL,RPC_C_AUTHN_LEVEL_DEFAULT,RPC_C_IMP_LEVEL_IMPERSONATE,NULL,EOAC_NONE,NULL)))
        {
            CoUninitialize();
            throw std::runtime_error("Failed to initialize security.");
        }
        IWbemLocator* locator;

        if(FAILED(CoCreateInstance(CLSID_WbemAdministrativeLocator,NULL,CLSCTX_INPROC_SERVER,IID_IWbemLocator,(LPVOID *) (&locator)))){
            CoUninitialize();
            throw std::runtime_error("Failed to create IWbemLocator object.");
        }

        IWbemServices *service = NULL;

        _bstr_t combined_target_namespace {wmi_namespace.c_str()};

        if (FAILED(locator->ConnectServer(combined_target_namespace,NULL,NULL,NULL,WBEM_FLAG_CONNECT_USE_MAX_WAIT,NULL,NULL,&service)))
        {
            locator->Release();
            delete service;
            CoUninitialize();
            throw std::runtime_error("Could not connect to " + combined_target_namespace + ".");
        }

        IEnumWbemClassObject* enumerator = NULL;
        if (FAILED(service->ExecQuery(L"WQL", bstr_t(wmi_query.c_str()),WBEM_FLAG_FORWARD_ONLY, NULL,&enumerator)))
        {
            service->Release();
            locator->Release();
            CoUninitialize();
            throw std::runtime_error("Query for operating system name failed.");
        }

        IWbemClassObject* wmiobjectrow = NULL;
        ULONG uReturn = 0;

        std::vector<std::vector<std::string>> wmi_result {};
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        
        while (enumerator)
        {
            if (FAILED(enumerator->Next(WBEM_INFINITE, 1L, &wmiobjectrow, &uReturn)))
            {
                service->Release();
                locator->Release();
                enumerator->Release();
                CoUninitialize();
                throw std::runtime_error("Failed to get next WbemClassObject.");
            }

            if(0 == uReturn)
            {
                break;
            }

            SAFEARRAY* sfArray {nullptr};
            LONG lstart, lend = 0;
            VARIANT vtProp {sizeof(VARIANT)};

            wmiobjectrow->GetNames(0,WBEM_FLAG_ALWAYS,0,&sfArray);

            if(FAILED(SafeArrayGetLBound( sfArray, 1, &lstart ))) return false;
            if(FAILED(SafeArrayGetUBound( sfArray, 1, &lend ))) return false;
            
            
            BSTR* pbstr;
            int i = 0;
            HRESULT hr = 0;
            
            if(SUCCEEDED(SafeArrayAccessData(sfArray,(void HUGEP**)&pbstr)))
            {
                if (wmi_result.size() == 0) {
                    std::vector<std::string> property_row {};
                    for (i = lstart; i < lend; i++) {
                        property_row.push_back(converter.to_bytes(pbstr[i]));
                    }
                    wmi_result.push_back(property_row);
                }

                CIMTYPE pType = 0;
                std::vector<std::string> wmi_row {};
                for(i=lstart; i < lend; i++)
                {		

                    wmiobjectrow->Get(pbstr[i], 0, &vtProp, &pType, 0);
                    if (vtProp.vt== VT_NULL)
                    {
                        wmi_row.push_back("");
                        continue;
                    }
                    
                    if (pType == CIM_STRING && pType != CIM_EMPTY && pType!= CIM_ILLEGAL  )
                    {
                        std::wstring raw_row {vtProp.bstrVal, SysStringLen(vtProp.bstrVal)};
                        wmi_row.push_back(converter.to_bytes(raw_row));
                    }
                    
                    VariantClear(&vtProp);
        
                }
                if (FAILED(SafeArrayUnaccessData(sfArray))) return hr;
                wmi_result.push_back(wmi_row);
            }	
        
            wmiobjectrow->Release();
        }

        service->Release();
        locator->Release();
        enumerator->Release();
        CoUninitialize();

        
        //this->output = wmi_result.;
        return true;
    }
    catch (const std::exception& e){
        this->handle_exceptions(e);
        return false;
    }
};

void WMIMonitorBlock::handle_exceptions(const std::exception e) {
    std::string caught_exception = e.what();
    this->output = "WMI execution failure: " + caught_exception;
};