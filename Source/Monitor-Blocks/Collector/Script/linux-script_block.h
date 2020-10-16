#pragma once

#include <filesystem>
#include <fstream>
#include <map>

#include "../collector_monitor_block.h"
#include "../../../utilitie s/linux-command_executor.h"

class ScriptMonitorBlock : public CollectorMonitorBlock {
    private:
        MonitorBlockOutput<std::map<std::string,std::string>> *output  = new MonitorBlockOutput<std::map<std::string,std::string>>;
        
        bool execute() override;
        void handle_exceptions(const std::exception e) override;
    public:
        ScriptMonitorBlock(const char* id,const char* name,const char* parameters);
        ~ScriptMonitorBlock();
};