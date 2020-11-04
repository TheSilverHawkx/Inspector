#pragma once

#include "..\collector_monitor_block.h"
#include "..\..\..\utilities\windows-command_executor.h"

#include <filesystem>
#include <fstream>
#include <tuple>
#include <vector>
#include <regex>

class ScriptMonitorBlock : public CollectorMonitorBlock {
    private:
        MonitorBlockOutput<clear_text_output> *output  = new MonitorBlockOutput<clear_text_output>;;

        bool execute() override;
        void handle_exceptions(const std::exception e) override;
    public:
        ScriptMonitorBlock(const char* id,const char* name,const char* parameters);
        ~ScriptMonitorBlock();

        
};