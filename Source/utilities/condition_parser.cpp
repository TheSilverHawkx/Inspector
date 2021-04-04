#include "condition_parser.h"
#include <iostream>

bool inspector::evaluate_condition(rapidjson::Value& conditions_list, std::vector<std::string>& output_list) {
	bool condition_status {false};
	std::string group_operator {""};

    // Check for Condition with "AND"/"OR" by looking for 'operator' keys.
	// Implies multiple conditions
    if (conditions_list.HasMember("group_operator")) {
		std::string group_operator = conditions_list["group_operator"].GetString();

		// Handle AND group operator. When gets at least 1 false then the whole group is false
		if (group_operator == "and") {
			for (auto& condition : conditions_list["conditions"].GetArray()) {
				//if (inspector::evaluate_condition(condition,output_list) == false) {
				//	return false;
				//}
			}
			return true;
		}
		// Handle OR group operator. When gets at least 1 true then the whole group is true
		else if (group_operator == "or") {
			for (auto& condition : conditions_list["conditions"].GetArray()) {
				/*if (inspector::evaluate_condition(condition,output_list)) {
					return true;
				}*/
			}
			return false;
		}
		else {
			throw "Invalid group operattor. found: " + group_operator;
		}
	}
	
	// Simple Condition
	std::string condition_operator = conditions_list["condition_operator"].GetString();
	std::string condition_subject = conditions_list["condition_value"].GetString();

	try {
	return inspector::condition_operator_table[condition_operator](output_list,condition_subject);
	}
	catch (std::exception e)
	{
		return false;
	}
}

std::map<std::string,std::function<bool(std::vector<std::string>&,std::string&)> > inspector::condition_operator_table {
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
