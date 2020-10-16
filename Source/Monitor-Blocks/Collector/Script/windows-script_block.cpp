#include "windows-script_block.h"


ScriptMonitorBlock::ScriptMonitorBlock(const char* id,const char* name,const char* parameters) :
CollectorMonitorBlock(id,name,parameters,_output_type::ClearText) {};

ScriptMonitorBlock::~ScriptMonitorBlock() {
    delete this->output;
};

bool ScriptMonitorBlock::execute() {
    try {
        // Parse Script Parameters
        const std::string& script_code = this->parameters["script_code"].GetString();
        const std::string& script_params = this->parameters["script_parameters"].GetString();
        const std::string& script_language = this->parameters["script_language"].GetString();

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
        
        auto [command_output,rc]= inspector::execute_command(cmd,timeout_in_seconds,work_directory_path);

        // Delete Work Folder
        if (! std::filesystem::remove_all(work_directory_path.c_str()))
        {
            throw std::runtime_error("Could not delete work folder");
        }

        // Format output according to this->_output_type
        *(this->output->data) = inspector::simplify_output(command_output,script_language);
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