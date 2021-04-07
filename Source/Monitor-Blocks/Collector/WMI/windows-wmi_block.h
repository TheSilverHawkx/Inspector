#pragma once

#include "..\collector_monitor_block.h"

#include <Windows.h>
#include <comutil.h>
#include <WbemIdl.h>
#include <comdef.h>
#include <codecvt>
#include <vector>
#include <locale>

#pragma comment(lib, "wbemuuid.lib")

class WMIMonitorBlock : public CollectorMonitorBlock {
    private:

        bool execute() override;
        void handle_exceptions(const std::exception e) override;
    public:
        WMIMonitorBlock(const char*,const char*,const char*);
        ~WMIMonitorBlock();
        
        MonitorBlockOutput<table_output> *output = new MonitorBlockOutput<table_output>;
};
