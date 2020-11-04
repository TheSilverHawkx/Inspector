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
    bool evaluate_condition(rapidjson::Document&,std::vector<std::string>&);
    bool evaluate_condition(rapidjson::Value&,std::vector<std::string>&);

    std::map<const char*,std::function<bool(std::vector<std::string>,std::string)>> operator_table  {
        {"equals",[](std::vector<std::string>& output_list,std::string& condition) {
                char* c;
                char* p;
                int condition_number = (int)std::strtol(condition.c_str(),&c,10);
                int output_number = (int)std::strtol(output_list[0].c_str(),&p,10);
                if (*p == 0 && *c == 0) {
                    return (output_number == condition_number) ? true : false;
                }
                else {
                    for (std::vector<std::string>::iterator it = output_list.begin(); it!= output_list.end();++it) {
                        if (condition == *it){
                            continue;
                        }
                        else {
                            return false;
                        }
                    }
                    return true;
                }
            }
        },
        {"notequals",[](std::vector<std::string>& output_list,std::string& condition) {
                char* c;
                char* p;
                int condition_number = (int)std::strtol(condition.c_str(),&c,10);
                int output_number = (int)std::strtol(output_list[0].c_str(),&p,10);
                if (*p == 0 && *c == 0) {
                    return (output_number != condition_number) ? true : false;
                }
                else {
                    for (std::vector<std::string>::iterator it = output_list.begin(); it!= output_list.end();++it) {
                        if (condition == *it){
                            return false;
                        }
                        else {
                            continue;
                        }
                    }
                    return true;
                }
            }
        },
        {"greaterthan",[](std::vector<std::string>& output_list,std::string& condition) {
                char* p;
                char* c;
                int condition_number = (int)std::strtol(condition.c_str(),&c,10);
                int output_number = (int)std::strtol(output_list[0].c_str(),&p,10);
                if (*p == 0 && *c == 0) {
                    return (output_number > condition_number) ? true : false;
                }
                else {
                    return false;
                }
            }
        },
        {"greaterequal",[](std::vector<std::string>& output_list,std::string& condition) {
                char* c;
                char* p;
                int condition_number = (int)std::strtol(condition.c_str(),&c,10);
                int output_number = (int)std::strtol(output_list[0].c_str(),&p,10);
                if (*p == 0 && *c == 0) {
                    return (output_number >= condition_number) ? true : false;
                }
                else {
                    return false;
                }            
            }
        },
        {"lessthan",[](std::vector<std::string>& output_list,std::string& condition) {
                char* c;
                char* p;
                int condition_number = (int)std::strtol(condition.c_str(),&c,10);
                int output_number = (int)std::strtol(output_list[0].c_str(),&p,10);
                if (*p == 0 && *c == 0) {
                    return (output_number < condition_number) ? true : false;
                }
                else {
                    return false;
                }             
            }
        },
        {"lessequal",[](std::vector<std::string>& output_list,std::string& condition) {
                char* c;
                char* p;
                int condition_number = (int)std::strtol(condition.c_str(),&c,10);
                int output_number = (int)std::strtol(output_list[0].c_str(),&p,10);
                if (*p == 0 && *c == 0) {
                    return (output_number <= condition_number) ? true : false;
                }
                else {
                    return false;
                }    
            }
        },
        {"contains",[](std::vector<std::string>& output_list,std::string& condition) {
                for (std::vector<std::string>::iterator it = output_list.begin(); it!= output_list.end();++it) {
                    if ((*it).find(condition) != std::string::npos){
                        return true;
                    }
                }
                return false;
            }
        },
        {"notcontains",[](std::vector<std::string>& output_list,std::string& condition) {
                for (std::vector<std::string>::iterator it = output_list.begin(); it!= output_list.end();++it) {
                    if ((*it).find(condition) != std::string::npos){
                        return false;
                    }
                }
                return true;
            }
        },
        {"regex",[](std::vector<std::string>& output_list,std::string& condition) {
                for (std::vector<std::string>::iterator it = output_list.begin(); it!= output_list.end();++it) {
                    if (std::regex_match(*it,std::regex(condition))){
                        return true;
                    }
                    else {
                        continue;
                    }
                }
                return false;
            }
        },
        {"notregex",[](std::vector<std::string>& output_list,std::string& condition) {
                for (std::vector<std::string>::iterator it = output_list.begin(); it!= output_list.end();++it) {
                    if (std::regex_match(*it,std::regex(condition))){
                        return false;
                    }
                    else {
                        continue;
                    }
                }
                return true;
            }
        }
        
    };
}