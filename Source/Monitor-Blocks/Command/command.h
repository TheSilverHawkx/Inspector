#ifndef _MONITOR_BLOCK_LINUX_COMMAND_
#define _MONITOR_BLOCK_LINUX_COMMAND_

#include <string>
#include "..\monitor_block.h"

class CommandMonitorBlock : public MonitorBlock {
    private:
        bool execute() override;
    public:
        CommandMonitorBlock(const char* id,const char* name,const char* parameters);
        ~CommandMonitorBlock();
};

#endif