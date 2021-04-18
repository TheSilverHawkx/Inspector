#pragma once

#include <string>

#ifdef _WIN32
    #include "..\..\..\Monitor-Blocks\all_monitor_blocks.h"
#else
    #include "../../../Monitor-Blocks/all_monitor_blocks.h"
#endif

struct workflow_item_struct {
    const char* id;
    const char* block_id;
    int next_id;

    // temporary
    _block_type mock_block_type;
    std::string mock_block_class;
};

class WorkflowItem {
    private:
        unsigned int item_id;
        const char* block_id;
        const char* workflow_id;
    public:
        WorkflowItem(unsigned int item_id,const char* block_id,const char* workflow_id,_block_type mock_type,std::string mock_class,MonitorBlock* previous_monitor_block = nullptr);
        ~WorkflowItem(); 

        MonitorBlock* monitor_block;

        bool run_item();
};
