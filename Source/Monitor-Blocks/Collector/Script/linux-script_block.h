#pragma once

#include <filesystem>
#include <fstream>
#include <map>

#include "../collector_monitor_block.h"
#include "../../../utilities/linux-command_executor.h"

class ScriptMonitorBlock : public CollectorMonitorBlock {
    private:
        MonitorBlockOutput<two_string_pair> *output  = new MonitorBlockOutput<two_string_pair>;
        
        bool execute() override;
        void handle_exceptions(const std::exception e) override;
    public:
        ScriptMonitorBlock(const char* id,const char* name,const char* parameters);
        ~ScriptMonitorBlock();
};