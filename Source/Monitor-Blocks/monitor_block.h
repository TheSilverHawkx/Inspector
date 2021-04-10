#pragma once

#include <string>
#include <stdexcept>

#ifdef _WIN32
    #include "..\..\include\rapidjson\document.h"
    #include "output_data_structure.h"
    #include "..\..\include\rapidjson\error\en.h"
#else
    #include "../../include/rapidjson/document.h"
    #include "output_data_structure.h"
    #include "../../include/rapidjson/error/en.h"
#endif

enum _block_type {trigger,collector,condition,operation,composite};
enum _output_type {Trigger,JSON,Table,Alert,PerformanceData,UpdateRequest,ClearText};

class MonitorBlock {
    protected:
        std::string id;
        std::string name;
        _block_type block_type;
        _output_type output_type;
        int execution_status;

        virtual bool execute() = 0;
        virtual void handle_exceptions(const std::exception e) = 0;
        rapidjson::Document* parse_json(const char*);

    public:
        void run_block();
};
