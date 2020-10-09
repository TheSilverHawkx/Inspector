#ifndef _MONITOR_BLOCK_WMI_BLOCK_
#define _MONITOR_BLOCK_WMI_BLOCK_

#include "..\..\monitor_block.h"
#include "..\..\output_data_structure.h"
#include <cstring>
#include <stdexcept>
#include <Windows.h>
#include <comutil.h>
#include <WbemIdl.h>
#include <objbase.h>
#include <comdef.h>
#include <codecvt>
#include <vector>
#include <locale>

#pragma comment(lib, "wbemuuid.lib")

class WMIMonitorBlock : public MonitorBlock {
    private:
        MonitorBlockOutput<std::vector<std::vector<std::string>>> *output = new MonitorBlockOutput<std::vector<std::vector<std::string>>>;

        bool execute() override;
        void handle_exceptions(const std::exception e) override;
    public:
        WMIMonitorBlock(const char*,const char*,const char*);
        ~WMIMonitorBlock();
};

#endif