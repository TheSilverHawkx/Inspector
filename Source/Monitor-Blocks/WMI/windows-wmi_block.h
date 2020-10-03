#ifndef _MONITOR_BLOCK_WMI_BLOCK_
#define _MONITOR_BLOCK_WMI_BLOCK_


#include "..\monitor_block.h"
#include <string>
#include <cstring>
#include <stdexcept>
#include <Windows.h>
#include <comutil.h>
#include <WbemIdl.h>
#include <objbase.h>
#include <comdef.h>

#pragma comment(lib, "wbemuuid.lib")

class WMIMonitorBlock : public MonitorBlock {
    private:
        bool execute() override;
        void handle_exceptions(const std::exception e) override;
    public:
        WMIMonitorBlock(const char*,const char*,const char*);
        ~WMIMonitorBlock();
};

#endif