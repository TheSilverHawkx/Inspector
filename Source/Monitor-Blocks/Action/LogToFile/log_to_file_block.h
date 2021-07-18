#pragma once

#ifdef _WIN32
    #include "..\action_monitor_block.h"
#else
    #include "../action_monitor_block.h"
#endif

#include <fstream>
#include <filesystem>

enum _log_mode { overwrite=1, append=2, binary=3};

class LogToFileBlock : public ActionMonitorBlock {
    private:
        std::ofstream file;
        void initialize_file(std::string, _log_mode);

    protected:
        bool execute();
        void handle_exceptions(const std::exception e);

    public:
        LogToFileBlock(const char*id,const char*parameters,MonitorBlock*);
        ~LogToFileBlock(); 
};