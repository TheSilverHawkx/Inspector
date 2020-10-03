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
        
        bool isAllProperties {wmi_query.find("*",0) != std::string::npos};


        HRESULT hres;
        if (FAILED(CoInitializeEx(0,COINIT_MULTITHREADED)))
        {
            throw std::runtime_error("Failed to initialize COM library.");
            return false;
        }

        if (FAILED(CoInitializeSecurity(NULL,-1,NULL,NULL,RPC_C_AUTHN_LEVEL_DEFAULT,RPC_C_IMP_LEVEL_IMPERSONATE,NULL,EOAC_NONE,NULL)))
        {
            throw std::runtime_error("Failed to initialize security.");
            CoUninitialize();
            return false;
        }
        IWbemLocator* locator;

        if(FAILED(CoCreateInstance(CLSID_WbemAdministrativeLocator,NULL,CLSCTX_INPROC_SERVER,IID_IWbemLocator,(LPVOID *) (&locator)))){
            throw std::runtime_error("Failed to create IWbemLocator object.");
            CoUninitialize();
            return false;
        }

        IWbemServices *service = NULL;

        std::string combined_target_namespace = "\\\\" + wmi_target + "\\" + wmi_namespace;
        if (FAILED(locator->ConnectServer(_bstr_t(combined_target_namespace.c_str()),NULL,NULL,NULL,WBEM_FLAG_CONNECT_USE_MAX_WAIT,NULL,NULL,&service)))
        {
            throw std::runtime_error("Could not connect to " + combined_target_namespace + ".");
            locator->Release();
            delete service;
            CoUninitialize();
            return false;
        }

        //hres = CoSetProxyBlanket(service,RPC_C_AUTHN_WINNT,RPC_C_AUTHZ_NONE,NULL,RPC_C_AUTHN_LEVEL_CALL,RPC_C_IMP_LEVEL_IMPERSONATE, NULL,EOAC_NONE);

        /*if (FAILED(hres))
        {
            throw std::runtime_error("Could not set proxy blanket.");
            service->Release();
            locator->Release();     
            CoUninitialize();
            return false;
        }*/

        IEnumWbemClassObject* enumerator = NULL;
        hres = service->ExecQuery(L"WQL", bstr_t(wmi_query.c_str()),WBEM_FLAG_FORWARD_ONLY, NULL,&enumerator);
        
        if (FAILED(hres))
        {
            throw std::runtime_error("Query for operating system name failed.");
            service->Release();
            locator->Release();
            CoUninitialize();
            return false;
        }

        IWbemClassObject* wmiobjectrow = NULL;
        ULONG uReturn = 0;

        while (enumerator)
        {
            
            HRESULT hr = enumerator->Next(WBEM_INFINITE, 1L, &wmiobjectrow, &uReturn);

            if(0 == uReturn)
            {
                break;
            }

            SAFEARRAY* sfArray;
            LONG lstart, lend;
            VARIANT vtProp;
            wmiobjectrow->GetNames(0,WBEM_FLAG_ALWAYS,0,&sfArray);
            hr = SafeArrayGetLBound( sfArray, 1, &lstart );
            if(FAILED(hr)) return false;
            hr = SafeArrayGetUBound( sfArray, 1, &lend );
            if(FAILED(hr)) return false;
            BSTR* pbstr;
            hr = SafeArrayAccessData(sfArray,(void HUGEP**)&pbstr);
            int nIdx =0;
            if(SUCCEEDED(hr))
            {
                CIMTYPE pType;
                for(nIdx=lstart; nIdx < lend; nIdx++)
                {		

                    hr = wmiobjectrow->Get(pbstr[nIdx], 0, &vtProp, &pType, 0);
                    if (vtProp.vt== VT_NULL)
                    {
                        continue;
                    }
                    if (pType == CIM_STRING && pType != CIM_EMPTY && pType!= CIM_ILLEGAL  )
                    {
                        std::string yeet =  " OS Name : "<< nIdx << vtProp.bstrVal <<endl;
                    }
                    
                    VariantClear(&vtProp);
        
                }
                hr = SafeArrayUnaccessData(sfArray);	
                if(FAILED(hr)) return hr;
            }	
        

            wmiobjectrow->Release();
        }

        service->Release();
        locator->Release();
        enumerator->Release();
        CoUninitialize();

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