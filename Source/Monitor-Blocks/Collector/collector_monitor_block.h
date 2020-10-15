#pragma once

#ifdef _WIN32
    #include "..\monitor_block.h"
#else
    #include "../monitor_block.h"
#endif

class CollectorMonitorBlock : virtual MonitorBlock {
    protected:
        std::string parameters;
        _output_type output_type;

        rapidjson::Document parse_parameters();

        virtual bool execute();
        virtual void handle_exceptions(const std::exception e);

    public:
        CollectorMonitorBlock(const char* id,const char* name,const char* parameters,_output_type output_type);
        ~CollectorMonitorBlock(); 
};
/*
 ,
				"${workspaceFolder}\\Source\\Monitor-Blocks\\Collector\\Command\\windows-*.cpp",
				"${workspaceFolder}\\Source\\Monitor-Blocks\\Collector\\Script\\windows-*.cpp",
				"${workspaceFolder}\\Source\\Monitor-Blocks\\Collector\\WMI\\windows-*.cpp",
				"${workspaceFolder}\\Source\\utilities\\windows-*.cpp"
*/