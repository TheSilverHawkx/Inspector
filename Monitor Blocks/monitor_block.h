#ifndef _MONITOR_BLOCK_
#define _MMONITOR_BLOCK_

#include <string>

enum _block_type {trigger,collector,condition,operation,composite};
enum _output_type {Trigger,JSON,CSV,Alert,PerformanceData,UpdateRequest};

class MonitorBlock {
    private:
        std::string id;
        std::string name;
        _block_type block_type;
        std::string parameters;
        _output_type output_type;
        int execution_status {-1};
        std::string output{};

    protected:
        virtual bool execute(void* output);

    public:
        MonitorBlock(const char* id,const char* name,_block_type type,const char* parameters,_output_type output_type);
        ~MonitorBlock();

        void run_block();
        
};

#endif