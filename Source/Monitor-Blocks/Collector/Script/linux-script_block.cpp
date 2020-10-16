#include "linux-script_block.h"


ScriptMonitorBlock::ScriptMonitorBlock(const char* id,const char* name,const char* parameters) :
CollectorMonitorBlock(id,name,parameters,_output_type::ClearText) {
    this->output->data->insert(std::make_pair("stdout",""));
    this->output->data->insert(std::make_pair("stderr",""));
    
};

ScriptMonitorBlock::~ScriptMonitorBlock() {
    delete this->output;
};

bool ScriptMonitorBlock::execute() {
    try {
        // Parse Information from parameters json
        const std::string& script_code = this->parameters["script_code"].GetString();
        const std::string& script_params = this->parameters["script_parameters"].GetString();
        const std::string& script_language = this->parameters["script_language"].GetString();

        // Define work directory and temporary script file
        const std::string folder_name = "workdir/" + this->id;
        const std::string file_name = folder_name + "/script_file";
        std::ofstream file;
        
        // Create Work Folder if doesn't exist
        if (!std::filesystem::exists(folder_name.c_str()))
        {
            if (!std::filesystem::create_directory(folder_name.c_str())){
                throw std::runtime_error("Could not create work folder");
            }
        }
        
        // Write script to temporary file
        file.open(file_name);
        if (file.fail()) throw std::runtime_error(std::string("Could not open script file for writing. Errno: " + errno));
        file << script_code << std::endl;
        file.close();

        // Run script using execute_command from linux-command_executor.h
        std::string cmd = script_language + " " + file_name + " " + script_params;
        auto [script_stdout,script_stderr,rc] = inspector::execute_command(cmd.c_str());


        // Delete Work Folder
        if (!std::filesystem::remove_all(folder_name.c_str()))
        {
            throw std::runtime_error("Could not delete work folder");
        }

        // Save output to output structure
        this->output->data->insert_or_assign("stdout",script_stdout);
        this->output->data->insert_or_assign("stderr",script_stderr);
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
    this->output->data->insert_or_assign("stderr","Script execution failure: " + caught_exception);
    this->output->return_code = -1;
};
