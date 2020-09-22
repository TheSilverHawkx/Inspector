#ifndef _MONITOR_BLOCK_LINUX_COMMAND_
#define _MONITOR_BLOCK_LINUX_COMMAND_

#include <string>
#include "monitor_block.h"

class LinuxCommandMonitorBlock : public MonitorBlock {
    private:
        bool execute(void* output) override;
    public:
        LinuxCommandMonitorBlock(const char* id,const char* name,_block_type type,const char* parameters,_output_type output_type);
        ~LinuxCommandMonitorBlock();
};

#endif