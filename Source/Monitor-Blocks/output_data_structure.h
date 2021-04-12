#pragma once

#include <map>
#include <vector>

//% use enum from monitor_block.h instead of typedef
typedef std::string clear_text_output;
typedef std::map<std::string,std::string> two_string_pair_output;
typedef std::vector<std::vector<std::string>> table_output;


template<typename T>
struct MonitorBlockOutput
{
    int return_code {};
    T *data = new T;
    
    ~MonitorBlockOutput() {
        delete data;
    }

    public:
    std::vector<std::string> to_list() {
        std::vector<std::string> list {};

        if constexpr (std::is_same_v<T,clear_text_output>) {
            std::string line = {""};
            for (clear_text_output::iterator it = this->data->begin(); it != this->data->end(); ++it){
                if (*it == '\n'){
                    list.push_back(line);
                    line = {};
                }
                else {
                    line += *it;
                }
            }
            if (!line.empty()) {
                list.push_back(line);
            }
        }
        else if constexpr (std::is_same_v<T,two_string_pair_output>)
        {
            for (two_string_pair_output::iterator it=this->data->begin(); it!=this->data->end();++it){
                list.push_back(std::string {it->first + ": " + it->second});
            }
        }
        else if constexpr (std::is_same_v<T,table_output>)
        {
            for (table_output::iterator it=this->data->begin(); it!=this->data->end();++it){
                for (std::vector<std::string>::iterator it2=it->begin();it2!=it->end();++it2)
                {
                    list.push_back((*it2));
                }
            }
        }

        return list;
    }
};

