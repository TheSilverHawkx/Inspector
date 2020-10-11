#pragma once

#include "..\..\monitor_block.h"
#include "..\..\..\utilities\windows-command_executor.h"
#include <stdio.h>
#include <tuple>
#include <filesystem>
#include <iostream>
// Needs rework

class CommandMonitorBlock : public MonitorBlock {
    private:
        MonitorBlockOutput<std::string> *output = new MonitorBlockOutput<std::string>;

        bool execute() override;
        void handle_exceptions(const std::exception e) override;
    public:
        CommandMonitorBlock(const char* id,const char* name,const char* parameters);
        ~CommandMonitorBlock();

};