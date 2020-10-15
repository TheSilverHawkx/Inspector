#pragma once

#include <string>
#include <stdexcept>

#ifdef _WIN32
    #include "..\..\include\rapidjson\document.h"
    #include "output_data_structure.h"
#else
    #include "../../include/rapidjson/document.h"
    #include "output_data_structure.h"
#endif

enum _block_type {trigger,collector,condition,operation,composite};
enum _output_type {Trigger,JSON,CSV,Alert,PerformanceData,UpdateRequest,ClearText};

class MonitorBlock {
    protected:
        std::string id;
        std::string name;
        _block_type block_type;
        _output_type output_type;
        int execution_status;


        //rapidjson::Document parse_parameters();

        virtual bool execute() = 0;
        virtual void handle_exceptions(const std::exception e) = 0;

    public:
        //MonitorBlock(const char* id,const char* name,_block_type type,const char* parameters,_output_type output_type);
        virtual ~MonitorBlock() =0; 

        void run_block();
};
