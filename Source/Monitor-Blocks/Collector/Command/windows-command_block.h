#pragma once

#include "..\collector_monitor_block.h"
#include "..\..\..\utilities\windows-command_executor.h"
#include <stdio.h>
#include <tuple>
#include <filesystem>

class CommandMonitorBlock : public CollectorMonitorBlock {
    private:
        MonitorBlockOutput<clear_text_output> *output = new MonitorBlockOutput<clear_text_output>;

        bool execute() override;
        void handle_exceptions(const std::exception e) override;
    public:
        CommandMonitorBlock(const char* id,const char* name,const char* parameters);
        ~CommandMonitorBlock();

};