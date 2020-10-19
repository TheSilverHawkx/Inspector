#pragma once

#ifdef _WIN32
    #include "..\..\include\rapidjson\rapidjson.h"
    #include "..\..\include\rapidjson\document.h"
#else
    #include "../../include/rapidjson/rapidjson.h"
    #include "../../include/rapidjson/document.h"
#endif

namespace inspector {
    bool evaluate_condition(rapidjson::Document&);
}