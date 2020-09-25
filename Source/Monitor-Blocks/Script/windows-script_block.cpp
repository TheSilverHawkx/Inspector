#include "windows-script_block.h"
#include <stdio.h>
#include <array>
#include <filesystem>
#include <fstream>

// ** File not working since <filesystem> is bugged as of mingw-w64 8.x ** //

ScriptMonitorBlock::ScriptMonitorBlock(const char* id,const char* name,const char* parameters) :
MonitorBlock(id,name,_block_type::collector,parameters,_output_type::ClearText) {};

ScriptMonitorBlock::~ScriptMonitorBlock() {};

bool ScriptMonitorBlock::execute() {
    try {
        FILE *command_output = NULL;
        rapidjson::Document parsed_parameters = this->parse_parameters();
        const std::string& script_code = parsed_parameters["script_code"].GetString();
        const std::string& script_params = parsed_parameters["script_parameters"].GetString();
        const std::string& script_language = parsed_parameters["script_language"].GetString();

        const std::string folder_name = "workdir/" + this->id;
        if (!std::filesystem::exists(folder_name.c_str()))
        {
            if (!std::filesystem::create_directory(folder_name.c_str())){
                throw std::runtime_error("Could not create work folder");
            }
        }
        
        
        const std::string file_name = folder_name + "/script_file";
        std::ofstream file;
        file.open(file_name);
        file << script_code << std::endl;
        file.close();

        
        if (script_language == "powershell") {

        }
        else if (script_language == "batch") {
            std::string cmd = script_code + " " + script_params;
            command_output = ::_popen(cmd.c_str(),"r");
        }
        else {
            throw std::runtime_error("unknown script language");
        }

        
        if (command_output == nullptr) {
            throw std::runtime_error("Cannot open script pipe");
        }

        std::array<char,256> buffer;

        while (! std::feof(command_output)) {
            auto bytes = std::fread(buffer.data(),1,buffer.size(),command_output);
            this->output.append(buffer.data(),bytes);
        }

        if (! std::filesystem::remove_all(folder_name.c_str()))
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
    this->output = "Script execution failure: " + caught_exception;
};