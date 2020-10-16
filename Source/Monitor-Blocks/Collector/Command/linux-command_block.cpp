#include "linux-command_block.h"

CommandMonitorBlock::CommandMonitorBlock(const char* id,const char* name,const char* parameters) :
CollectorMonitorBlock(id,name,parameters,_output_type::ClearText) {
    this->output->data->insert(std::make_pair("stdout",""));
    this->output->data->insert(std::make_pair("stderr",""));
};

CommandMonitorBlock::~CommandMonitorBlock() {
    delete this->output;
};

bool CommandMonitorBlock::execute() {
    try {
        // Define work directory
        const std::string folder_name = "workdir/" + this->id;

        // Create Work Folder if doesn't exist
        if (!std::filesystem::exists(folder_name.c_str()))
        {
            if (!std::filesystem::create_directory(folder_name.c_str())){
                throw std::runtime_error("Could not create work folder");
            }
        }

        std::string cmd = "cd " + folder_name + "; " + (std::string)this->parameters["commandline"].GetString();
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
    catch (const std::exception& e) {
        this->handle_exceptions(e);
        return false;
    }
};

void CommandMonitorBlock::handle_exceptions(const std::exception e) {
    std::string caught_exception = e.what();
    this->output->data->insert_or_assign("stderr","Script execution failure: " + caught_exception);
    this->output->return_code = -1;
};