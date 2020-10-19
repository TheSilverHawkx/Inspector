#pragma once

#ifdef _WIN32
    #include "..\monitor_block.h"
#else
    #include "../monitor_block.h"
#endif

template <typename T>
class ConditionMonitorBlock : public MonitorBlock {
    protected:
        MonitorBlockOutput<T> Incomingdata;
        
        rapidjson::Document parameters;
        

        virtual bool execute();
        virtual void handle_exceptions(const std::exception e);

    public:
        ConditionMonitorBlock(const char* id,const char* name,const char* parameters);
        ~ConditionMonitorBlock(); 
};