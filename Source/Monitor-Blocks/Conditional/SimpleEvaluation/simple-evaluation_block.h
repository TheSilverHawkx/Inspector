#pragma once

#ifdef _WIN32
    #include "..\conditional_monitor_block.h"
#else
    #include "../conditional_monitor_block.h"
#endif

class SimpleEvaluationBlock : public ConditionalMonitorBlock {
    protected:
        bool execute();
        void handle_exceptions(const std::exception e);

    public:
        SimpleEvaluationBlock(const char*id,const char*parameters,CollectorMonitorBlock*);
        ~SimpleEvaluationBlock(); 
};