#pragma once

#include "../../monitor_block.h"
#include "../../../utilities/linux-command_executor.h"
#include <map>
//#include <stdio.h>
#include <filesystem>

class CommandMonitorBlock : public MonitorBlock {
    private:
        MonitorBlockOutput<std::map<std::string,std::string>> *output  = new MonitorBlockOutput<std::map<std::string,std::string>>;

        bool execute() override;
        void handle_exceptions(const std::exception e) override;
    public:
        CommandMonitorBlock(const char*,const char*,const char*);
        ~CommandMonitorBlock();
};