#ifndef _MONITOR_BLOCK_COMMAND_BLOCK_
#define _MONITOR_BLOCK_COMMAND_BLOCK_

#include "..\..\monitor_block.h"
#include <stdio.h>
#include <array>
#include <stdexcept>

class CommandMonitorBlock : public MonitorBlock {
    private:
        MonitorBlockOutput<std::string> *output = new MonitorBlockOutput<std::string>;

        bool execute() override;
        void handle_exceptions(const std::exception e) override;
    public:
        CommandMonitorBlock(const char* id,const char* name,const char* parameters);
        ~CommandMonitorBlock();
};

#endif