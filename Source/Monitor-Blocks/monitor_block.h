#ifndef _MONITOR_BLOCK_
#define _MMONITOR_BLOCK_

#include <string>
#ifdef _WIN32
    #include "..\..\include\rapidjson\document.h"
#else
    #include "../../include/rapidjson/document.h"
#endif

enum _block_type {trigger,collector,condition,operation,composite};
enum _output_type {Trigger,JSON,CSV,Alert,PerformanceData,UpdateRequest,ClearText};

class MonitorBlock {
    protected:
        std::string id;
        std::string name;
        _block_type block_type;
        std::string parameters;
        _output_type output_type;
        int execution_status {-1};
        std::string output{""};


        rapidjson::Document parse_parameters();

        virtual bool execute();
        virtual void handle_exceptions(const std::exception e);

    public:
        MonitorBlock(const char* id,const char* name,_block_type type,const char* parameters,_output_type output_type);
        ~MonitorBlock();

        void run_block();

        std::string print_output();
        
};

#endif