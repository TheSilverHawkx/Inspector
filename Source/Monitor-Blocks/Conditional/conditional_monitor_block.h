#pragma once

#ifdef _WIN32
    #include "..\monitor_block.h"
    #include "..\..\utilities\condition_parser.h"
    #include "..\Collector\WMI\windows-wmi_block.h"
    #include "..\Collector\Script\windows-script_block.h"
    #include "..\Collector\Command\windows-command_block.h"
#else
    #include "../monitor_block.h"
    #include "../../utilities/condition_parser.h"
    #include "../Collector/Command/linux-command_block.h"
    #include "../Collector/Script/linux-script_block.h"
#endif

class ConditionalMonitorBlock : public MonitorBlock {
    protected:
        rapidjson::Document* parameters;
        std::vector<std::string> collected_data;

        virtual bool execute();
        virtual void handle_exceptions(const std::exception e);

    public:
        ConditionalMonitorBlock(const char*,const char*,CollectorMonitorBlock*);
        ~ConditionalMonitorBlock(); 
        
        bool evaluation_status;
        std::vector<std::string> get_collected_data();
        
};