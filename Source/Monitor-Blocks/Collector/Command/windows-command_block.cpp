#include "windows-command_block.h"
#include "..\..\..\utilities\windows-command_executor.h"

CommandMonitorBlock::CommandMonitorBlock(const char* id,const char* name,const char* parameters) :
MonitorBlock(id,name,_block_type::collector,parameters,_output_type::ClearText) {};

CommandMonitorBlock::~CommandMonitorBlock() {
    delete this->output;
};

bool CommandMonitorBlock::execute() {
    try {
        const std::string work_directory_path = std::filesystem::current_path().string() + "\\workdir\\" + this->id;
        std::string cmd { "cmd /c \"" + this->parameters + "\""};
        int timeout_in_seconds = 10;

        // Create Work Folder if doesn't exist
        if (!std::filesystem::exists(work_directory_path.c_str()))
        {
            if (!std::filesystem::create_directory(work_directory_path.c_str())){
                throw std::runtime_error("Could not create work folder");
            }
        }

        auto [command_output,rc] = inspector::execute_command(cmd,timeout_in_seconds,work_directory_path);

        // Delete Work Folder
        if (! std::filesystem::remove_all(work_directory_path.c_str()))
        {
            throw std::runtime_error("Could not delete work folder");
        }

        *(this->output->data) = inspector::simplify_output(command_output,"batch");
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
    *(this->output->data) = "Command execution failure: " + caught_exception;
    this->output->return_code = -1;
};