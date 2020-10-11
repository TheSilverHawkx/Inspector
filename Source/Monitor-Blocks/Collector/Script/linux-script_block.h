#pragma once

#include <filesystem>
#include <fstream>
#include <map>

#include "../../monitor_block.h"
#include "../../../utilities/linux-command_executor.h"

class ScriptMonitorBlock : public MonitorBlock {
    private:
        MonitorBlockOutput<std::map<std::string,std::string>> *output  = new MonitorBlockOutput<std::map<std::string,std::string>>;
        
        bool execute() override;
        void handle_exceptions(const std::exception e) override;
    public:
        ScriptMonitorBlock(const char* id,const char* name,const char* parameters);
        ~ScriptMonitorBlock();
};