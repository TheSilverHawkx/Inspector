#ifndef _MONITOR_BLOCK_WMI_BLOCK_
#define _MONITOR_BLOCK_WMI_BLOCK_


#include <string>


#include "..\monitor_block.h"

class WMIMonitorBlock : public MonitorBlock {
    private:
        bool execute() override;
        void handle_exceptions(const std::exception e) override;
    public:
        WMIMonitorBlock(const char* id,const char* name,const char* parameters);
        ~WMIMonitorBlock();
};

#endif