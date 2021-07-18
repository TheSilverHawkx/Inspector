#pragma once

#ifdef _WIN32
    #include "..\monitor_block.h"
    #include "..\..\utilities\condition_parser.h"
    #include "..\all_collector_blocks.h"
#else
    #include "../monitor_block.h"
    #include "../../utilities/condition_parser.h"
    #include "../all_collector_blocks.h"
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
};