#pragma once

#ifdef _WIN32
    #include "..\action_monitor_block.h"
    #include "..\..\..\Utilities\DBConnector\Agent\db_connector.h"
#else
    #include "../action_monitor_block.h"
    #include "../../../Utilities/DBConnector/Agent/db_connector.h"
#endif

#include <fstream>

enum _severity {resolved=0,information=1,warning=2,error=3,critical=4};

class CreateAlertBlock : public ActionMonitorBlock {
    private:
        _severity severity;
        std::string alert_name {};
        std::string serialize_collected_data();

    protected:
        bool execute();
        void handle_exceptions(const std::exception e);

    public:
        CreateAlertBlock(const char*id,const char*parameters,MonitorBlock*);
        ~CreateAlertBlock(); 
};