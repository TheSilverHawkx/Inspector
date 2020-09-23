#include "script_block.h"
#include <stdio.h>
#include <array>


ScriptMonitorBlock::ScriptMonitorBlock(const char* id,const char* name,const char* parameters) :
MonitorBlock(id,name,_block_type::collector,parameters,_output_type::ClearText) {};

ScriptMonitorBlock::~ScriptMonitorBlock() {};

bool ScriptMonitorBlock::execute() {
    try {
        FILE *command_output = NULL;
        rapidjson::Document parsed_parameters = this->parse_parameters();
        rapidjson::Value& script_code = parsed_parameters["script_code"];
        rapidjson::Value& script_params = parsed_parameters["script_parameters"];
        rapidjson::Value& script_language = parsed_parameters["script_language"];

        #ifdef _WIN32
        
            if (script_language.GetString() == "powershell") {

            }
            else if (script_language.GetString() == "batch") {
                command_output = ::_popen(script_code.GetString() + " " + script_params.GetString(),"r");
            }
            else {
                throw new std::runtime_error("unknown script language");
            }
            
        #else
            command_output = ::popen(this->parameters.c_str(),"r");
        #endif
        
        if (command_output == nullptr) {
            throw new std::runtime_error("Cannot open shell pipe");
        }

        std::array<char,256> buffer;

        while (not std::feof(command_output)) {
            auto bytes = std::fread(buffer.data(),1,buffer.size(),command_output);
            this->output.append(buffer.data(),bytes);
        }

        return true;
    }
    catch (const std::exception& e){
        this->handle_exceptions(e);
        return false;
    }
};

void ScriptMonitorBlock::handle_exceptions(const std::exception& e) {
    std::string caught_exception = e.what();
    this->output = "Script execution failure: " + caught_exception;
};