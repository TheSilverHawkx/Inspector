#ifndef _MONITOR_BLOCK_
#define _MMONITOR_BLOCK_

#include <string>

enum _block_type {trigger,collector,condition,operation,composite};
enum _output_type {Trigger,JSON,CSV,Alert,PerformanceData,UpdateRequest,ClearText};

class MonitorBlock {
    protected:
        std::string id;
        std::string name;
        _block_type block_type;
        std::string parameters;
        _output_type output_type;
        int execution_status {-1};
        std::string output{};
        virtual bool execute();

    public:
        MonitorBlock(const char* id,const char* name,_block_type type,const char* parameters,_output_type output_type);
        ~MonitorBlock();

        void run_block();

        std::string print_output();
        
};

#endif