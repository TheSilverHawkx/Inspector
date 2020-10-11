#pragma once

#include "..\..\monitor_block.h"
#include "..\..\..\utilities\windows-command_executor.h"

#include <filesystem>
#include <fstream>
#include <tuple>
#include <vector>
#include <regex>

class ScriptMonitorBlock : public MonitorBlock {
    private:
        MonitorBlockOutput<std::string> *output  = new MonitorBlockOutput<std::string>;;

        bool execute() override;
        void handle_exceptions(const std::exception e) override;
        std::string simplify_output(const std::string&,const std::string&);
    public:
        ScriptMonitorBlock(const char* id,const char* name,const char* parameters);
        ~ScriptMonitorBlock();

        
};