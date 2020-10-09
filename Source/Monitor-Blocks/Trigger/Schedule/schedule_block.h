#pragma once

#ifdef _WIN32
    #include "..\..\monitor_block.h"
    #include "..\..\output_data_structure.h"
#else
    #include "../../monitor_block.h"
    #include "../../output_data_structure.h"
#endif

#include <chrono>
#include <stdexcept>

class ScheduleMonitorBlock : public MonitorBlock {
    private:
        bool execute() override;
        void handle_exceptions(const std::exception e) override;
    public:
        ScheduleMonitorBlock(const char* id,const char* name,const char* parameters);
        ~ScheduleMonitorBlock();
};