#ifndef _MONITOR_BLOCK_COMMAND_BLOCK_
#define _MONITOR_BLOCK_COMMAND_BLOCK_

#include <string>
#include <map>
#include <stdio.h>
#include <filesystem>
#include "../monitor_block.h"
#include "../output_data_structure.h"
#include "../../utilities/linux-command_executor.h"

class CommandMonitorBlock : public MonitorBlock {
    private:
        MonitorBlockOutput<std::map<std::string,std::string>> *output  = new MonitorBlockOutput<std::map<std::string,std::string>>;

        bool execute() override;
        void handle_exceptions(const std::exception e) override;
    public:
        CommandMonitorBlock(const char*,const char*,const char*);
        ~CommandMonitorBlock();
};

#endif