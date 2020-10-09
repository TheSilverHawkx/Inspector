#ifndef _MONITOR_BLOCK_SCRIPT_BLOCK_
#define _MONITOR_BLOCK_SCRIPT_BLOCK_

#include <string>
#include <regex>
#include "..\monitor_block.h"
#include "..\output_data_structure.h"

class ScriptMonitorBlock : public MonitorBlock {
    private:
        MonitorBlockOutput<std::string> *output  = new MonitorBlockOutput<std::string>;;

        bool execute() override;
        void handle_exceptions(const std::exception e) override;
        std::string simplify_output(const std::string&,const std::string&);
    public:
        ScriptMonitorBlock(const char* id,const char* name,const char* parameters);
        ~ScriptMonitorBlock();

        
};

#endif