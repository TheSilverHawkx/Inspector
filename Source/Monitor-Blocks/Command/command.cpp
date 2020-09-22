#include "command.h"
#include <stdio.h>
#include <array>


CommandMonitorBlock::CommandMonitorBlock(const char* id,const char* name,const char* parameters) :
MonitorBlock(id,name,_block_type::collector,parameters,_output_type::ClearText) {};

CommandMonitorBlock::~CommandMonitorBlock() {};

bool CommandMonitorBlock::execute() {
    FILE *command_output = NULL;

    try {
        #ifdef _WIN32
            command_output = ::_popen(this->parameters.c_str(),"r");
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
    catch (const std::exception& e) {
        std::string caught_exception = e.what();
        this->output = "Command execution failure: " + caught_exception;
        return false;
    }


};
