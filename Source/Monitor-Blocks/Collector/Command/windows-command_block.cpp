#include "windows-command_block.h"


CommandMonitorBlock::CommandMonitorBlock(const char* id,const char* name,const char* parameters) :
MonitorBlock(id,name,_block_type::collector,parameters,_output_type::ClearText) {};

CommandMonitorBlock::~CommandMonitorBlock() {};

bool CommandMonitorBlock::execute() {
    FILE *command_output = NULL;

    try {
        command_output = ::_popen(this->parameters.c_str(),"r");

        
        if (command_output == nullptr) {
            throw new std::runtime_error("Cannot open shell pipe");
        }

        std::array<char,256> buffer;

        while (!std::feof(command_output)) {
            auto bytes = std::fread(buffer.data(),1,buffer.size(),command_output);
            //*(this->output->data).append(buffer.data(),bytes);
        }

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