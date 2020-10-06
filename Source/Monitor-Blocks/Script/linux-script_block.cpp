#include "linux-script_block.h"
#include <stdio.h>
#include <array>
#include <filesystem>
#include <fstream>

ScriptMonitorBlock::ScriptMonitorBlock(const char* id,const char* name,const char* parameters) :
MonitorBlock(id,name,_block_type::collector,parameters,_output_type::ClearText) {};

ScriptMonitorBlock::~ScriptMonitorBlock() {
    delete this->output;
};

bool ScriptMonitorBlock::execute() {
    try {
        FILE *command_output = NULL;
        rapidjson::Document parsed_parameters = this->parse_parameters();
        const std::string& script_code = parsed_parameters["script_code"].GetString();
        const std::string& script_params = parsed_parameters["script_parameters"].GetString();
        const std::string& script_language = parsed_parameters["script_language"].GetString();

        const std::string folder_name = "workdir/" + this->id;
        if (not std::filesystem::exists(folder_name.c_str()))
        {
            if (not std::filesystem::create_directory(folder_name.c_str())){
                throw std::runtime_error("Could not create work folder");
            }
        }
        const std::string file_name = folder_name + "/script_file";
        std::ofstream file;
        file.open(file_name);
        file << script_code << std::endl;
        file.close();

        std::string cmd = script_language + " " + file_name + " " + script_params;
        auto [script_stdout,script_stderr,rc] = execute_commnad(cmd.c_str());

        if (script_language == "bash") {
            std::string cmd = "/bin/bash " + file_name + " " + script_params;
            command_output = ::popen(cmd.c_str(),"r");
        }
        else if(script_language.rfind("python") == 0) {
            std::string cmd = script_language + " " + file_name + " " + script_params;
            command_output = ::popen(cmd.c_str(),"r");
        }
        else {
            throw std::runtime_error("unknown script language");
        }
        
        if (command_output == nullptr) {
            throw std::runtime_error("Cannot open script pipe");
        }

        std::array<char,256> buffer;

        while (not std::feof(command_output)) {
            auto bytes = std::fread(buffer.data(),1,buffer.size(),command_output);
            //*(this->output->data).append(buffer.data(),bytes);
        }

        if (not std::filesystem::remove_all(folder_name.c_str()))
        {
            throw std::runtime_error("Could not delete work folder");
        }
        return true;
    }
    catch (const std::exception& e){
        this->handle_exceptions(e);
        return false;
    }
};

void ScriptMonitorBlock::handle_exceptions(const std::exception e) {
    std::string caught_exception = e.what();
    *(this->output->data) = "Script execution failure: " + caught_exception;
    this->output->return_code = -1;
};
