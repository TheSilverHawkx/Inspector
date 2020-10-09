#ifndef _MONITOR_BLOCK_SCRIPT_BLOCK_
#define _MONITOR_BLOCK_SCRIPT_BLOCK_

#include "..\..\monitor_block.h"
#include "..\..\..\utilities\windows-command_executor.h"
#include <stdio.h>
#include <filesystem>
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
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

#endif