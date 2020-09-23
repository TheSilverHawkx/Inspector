#ifndef _MONITOR_BLOCK_SCRIPT_BLOCK_
#define _MONITOR_BLOCK_SCRIPT_BLOCK_

#include <string>
#ifdef _WIN32
    #include "..\monitor_block.h"
#else
    #include "../monitor_block.h"
#endif

class ScriptMonitorBlock : public MonitorBlock {
    private:
        bool execute() override;
        void handle_exceptions(const std::exception e) override;
    public:
        ScriptMonitorBlock(const char* id,const char* name,const char* parameters);
        ~ScriptMonitorBlock();
};

#endif