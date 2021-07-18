#pragma once

#ifdef _WIN32
    #include "..\monitor_block.h"
    #include "..\all_collector_blocks.h"
    #include "..\all_condition_blocks.h"
#else
    #include "../monitor_block.h"
    #include "../all_collector_blocks.h"
    #include "../all_condition_blocks.h"
#endif

#include <regex>

class ActionMonitorBlock : public MonitorBlock {
    protected:
        rapidjson::Document* parameters;
        std::vector<std::string> collected_data;

        virtual bool execute();
        virtual void handle_exceptions(const std::exception e);
        std::string format_text();

    public:
        ActionMonitorBlock(const char* id,const char* parameters, MonitorBlock*);
        ~ActionMonitorBlock(); 
        
};