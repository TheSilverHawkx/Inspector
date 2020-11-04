#pragma once

#include "../collector_monitor_block.h"
#include "../../../utilities/linux-command_executor.h"
#include <map>
#include <filesystem>

class CommandMonitorBlock : public CollectorMonitorBlock {
    private:
        MonitorBlockOutput<two_string_pair> *output  = new MonitorBlockOutput<two_string_pair>;

        bool execute() override;
        void handle_exceptions(const std::exception e) override;
    public:
        CommandMonitorBlock(const char*,const char*,const char*);
        ~CommandMonitorBlock();
};