#ifndef _MONITOR_BLOCK_SCRIPT_BLOCK_
#define _MONITOR_BLOCK_SCRIPT_BLOCK_

#include <string>
#include <regex>
#include "..\monitor_block.h"


class ScriptMonitorBlock : public MonitorBlock {
    private:
        bool execute() override;
        void handle_exceptions(const std::exception e) override;
        std::string format_output(const std::string&,const std::string&);
    public:
        ScriptMonitorBlock(const char* id,const char* name,const char* parameters);
        ~ScriptMonitorBlock();

        
};

#endif