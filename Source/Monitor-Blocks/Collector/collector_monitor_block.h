#pragma once

#ifdef _WIN32
    #include "..\monitor_block.h"
#else
    #include "../monitor_block.h"
#endif

class CollectorMonitorBlock : public MonitorBlock {
    protected:
        rapidjson::Document* parameters;

        virtual bool execute();
        virtual void handle_exceptions(const std::exception e);

    public:
        _output_type output_type;
        
        CollectorMonitorBlock(const char* id,const char* name,const char* parameters,_output_type output_type);
        ~CollectorMonitorBlock();
        rapidjson::Document* get_parameters();
};