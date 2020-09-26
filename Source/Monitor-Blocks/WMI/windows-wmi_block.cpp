#include "windows-wmi_block.h"
#include <stdexcept>
#include <comdef.h>
#include <Wbemidl.h>
#include <cstring>

#pragma comment(lib, "wbemuuid.lib")


WMIMonitorBlock::WMIMonitorBlock(const char* id,const char* name,const char* parameters) :
MonitorBlock(id,name,_block_type::collector,parameters,_output_type::ClearText) {};

WMIMonitorBlock::~WMIMonitorBlock() {};

bool WMIMonitorBlock::execute() {
    try {
        rapidjson::Document parsed_parameters = this->parse_parameters();
        const std::string& wmi_namespace = parsed_parameters["namespace"].GetString();
        const std::string& wmi_query = parsed_parameters["query"].GetString();
        const std::string& wmi_target = parsed_parameters["target"].GetString();
        
        HRESULT hres;
        hres =  CoInitializeEx(0, COINIT_MULTITHREADED); 
        if (FAILED(hres))
        {
            throw std::runtime_error("Failed to initialize COM library.");
            return false;
        }

        hres =  CoInitializeSecurity(NULL,-1,NULL,NULL,RPC_C_AUTHN_LEVEL_DEFAULT,RPC_C_IMP_LEVEL_IMPERSONATE,NULL,EOAC_NONE,NULL);

                      
        if (FAILED(hres))
        {
            throw std::runtime_error("Failed to initialize security.");
            CoUninitialize();
            return false;
        }
        
        IWbemLocator *pLoc = NULL;

        hres = CoCreateInstance(CLSID_WbemLocator,0,CLSCTX_INPROC_SERVER,IID_IWbemLocator, (LPVOID *) &pLoc);
    
        if (FAILED(hres))
        {
            throw std::runtime_error("Failed to create IWbemLocator object.");
            CoUninitialize();
            return false;
        }

        IWbemServices *pSvc = NULL;

        std::string combined_target_namespace = "\\\\" + wmi_target + wmi_namespace;
        hres = pLoc->ConnectServer(_bstr_t(combined_target_namespace.c_str()),NULL,NULL,0,NULL,0,0,&pSvc);
        
        if (FAILED(hres))
        {
            throw std::runtime_error("Could not connect to \\\\" + combined_target_namespace + ".");
            pLoc->Release();     
            CoUninitialize();
            return false;
        }

        hres = CoSetProxyBlanket(pSvc,RPC_C_AUTHN_WINNT,RPC_C_AUTHZ_NONE,NULL,RPC_C_AUTHN_LEVEL_CALL,RPC_C_IMP_LEVEL_IMPERSONATE, NULL,EOAC_NONE);

        if (FAILED(hres))
        {
            throw std::runtime_error("Could not set proxy blanket.");
            pSvc->Release();
            pLoc->Release();     
            CoUninitialize();
            return false;
        }

        IEnumWbemClassObject* pEnumerator = NULL;
        hres = pSvc->ExecQuery(bstr_t("WQL"), bstr_t(wmi_query.c_str()),WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL,&pEnumerator);
        
        if (FAILED(hres))
        {
            throw std::runtime_error("Query for operating system name failed.");
            pSvc->Release();
            pLoc->Release();
            CoUninitialize();
            return false;
        }

        IWbemClassObject *pclsObj = NULL;
        ULONG uReturn = 0;
    
        while (pEnumerator)
        {
            SafeArrayGetLBound
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            if(0 == uReturn)
            {
                break;
            }

            VARIANT vtProp;

            // Get the value of the Name property
            hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
            VariantClear(&vtProp);

            pclsObj->Release();
        }

        pSvc->Release();
        pLoc->Release();
        pEnumerator->Release();
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