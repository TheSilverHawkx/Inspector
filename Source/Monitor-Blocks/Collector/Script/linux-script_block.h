#pragma once

#include <filesystem>
#include <fstream>
#include <map>

#include "../collector_monitor_block.h"
#include "../../../utilities/linux-command_executor.h"
#include "../../output_data_structure.h"

class ScriptMonitorBlock : public CollectorMonitorBlock {
    private:
        
        bool execute() override;
        void handle_exceptions(const std::exception e) override;
    public:
        ScriptMonitorBlock(const char* id,const char* name,const char* parameters);
        ~ScriptMonitorBlock();
        
        MonitorBlockOutput<two_string_pair_output> *output  = new MonitorBlockOutput<two_string_pair_output>;
};