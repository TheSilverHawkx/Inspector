#pragma once

#include <string>
#include <map>
#include <functional>
#include <vector>
#include <regex>

#ifdef _WIN32
    #include "..\..\include\rapidjson\rapidjson.h"
    #include "..\..\include\rapidjson\document.h"
#else
    #include "../../include/rapidjson/rapidjson.h"
    #include "../../include/rapidjson/document.h"
#endif

namespace inspector {
    bool evaluate_condition(rapidjson::Value&,std::vector<std::string>&);

    extern std::map<std::string,std::function<bool(std::vector<std::string>&,std::string&)> > condition_operator_table;
}