#include "windows-script_block.h"
#include "..\..\utilities\windows-command_executor.h"
#include <stdio.h>
#include <array>
#include <filesystem>
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <tuple>
#include <vector>
#include <regex>


ScriptMonitorBlock::ScriptMonitorBlock(const char* id,const char* name,const char* parameters) :
MonitorBlock(id,name,_block_type::collector,parameters,_output_type::ClearText) {};

ScriptMonitorBlock::~ScriptMonitorBlock() {
    delete this->output;
};

bool ScriptMonitorBlock::execute() {
    try {
        // Parse Script Parameters
        rapidjson::Document parsed_parameters = this->parse_parameters();
        const std::string& script_code = parsed_parameters["script_code"].GetString();
        const std::string& script_params = parsed_parameters["script_parameters"].GetString();
        const std::string& script_language = parsed_parameters["script_language"].GetString();

        // Declare Script-specific variables
        int timeout_in_seconds = 10;
        std::string cmd {};
        const std::string work_directory_path = std::filesystem::current_path().string() + "\\workdir\\" + this->id;
        std::string script_file_path = work_directory_path + "\\script_file";
        std::ofstream file {};

        // Create work directory if needed
        if (!std::filesystem::exists(work_directory_path.c_str()))
        {
            if (!std::filesystem::create_directory(work_directory_path.c_str())){
                throw std::runtime_error("Could not create work folder");
            }
        }

        // Handle script language
        if (script_language == "batch") {
            script_file_path +=".bat";
            cmd = "cmd /c \"" + script_file_path + "\" " + script_params;
        }
        else if (script_language == "powershell") {
            script_file_path +=".ps1";
            cmd = "powershell -noprofile -ExecutionPolicy Bypass -file \"" + script_file_path + "\" " + script_params;
        }
        else {
            throw std::runtime_error("unknown script language");
        } 

        // Create script file
        file.open(script_file_path);
        file << script_code << std::endl;
        file.close();
        
        auto [command_output,rc]= execute_commnad(cmd,timeout_in_seconds);

        // Delete Work Folder
        if (! std::filesystem::remove_all(work_directory_path.c_str()))
        {
            throw std::runtime_error("Could not delete work folder");
        }

        // Format output according to this->_output_type
        *(this->output->data) = this->simplify_output(command_output,script_language);
        this->output->return_code = rc;
        
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

std::string ScriptMonitorBlock::simplify_output(const std::string& raw_output,const std::string& language) {
    std::string parsed_output {};
    if (language == "batch") {
        std::string script_name = raw_output.substr(raw_output.find("\r\n",0)+2,raw_output.find(">",0)-1);

        std::regex delimiter{"\r\n"};
        std::vector<std::string> lines {
            std::sregex_token_iterator(raw_output.begin(),raw_output.end(),delimiter,-1), {}
        };

        for (auto& line:lines) {
            if (!line.empty() & std::string::npos == line.find(script_name,0))
            {
                parsed_output += line + "\n";
            }
        }
                
        return parsed_output.substr(0,parsed_output.length()-1) ;
    }
    else if (language == "powershell") {
        return raw_output.substr(0,raw_output.length()-1);
    }
    else {
        return "";
    }
};