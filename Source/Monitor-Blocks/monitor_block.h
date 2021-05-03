#pragma once

#include <string>
#include <stdexcept>
#include <typeinfo>

#include "output_data_structure.h"

#ifdef _WIN32
    #include "..\..\include\rapidjson\document.h"
    #include "..\..\include\rapidjson\error\en.h"
    #include "..\Utilities\Exceptions.h"
#else
    #include "../../include/rapidjson/document.h"
    #include "../../include/rapidjson/error/en.h"
    #include "../Utilities/Exceptions.h"
#endif

enum class _block_type {trigger =1,collector = 2,condition = 3,operation = 4,composite = 5};
enum class _output_type {Trigger,JSON,Table,Alert,PerformanceData,UpdateRequest,ClearText};

class MonitorBlock {
    protected:
        std::string id;
        std::string name;
        _output_type output_type;

        virtual bool execute() = 0;
        virtual void handle_exceptions(const std::exception e) = 0;
        rapidjson::Document* parse_json(const char*);

    public:
        _block_type block_type;
    
        bool run_block();
        
};
