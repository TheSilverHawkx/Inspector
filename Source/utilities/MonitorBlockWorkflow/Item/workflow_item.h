#pragma once

#include <string>

#ifdef _WIN32
    #include "..\..\..\Monitor-Blocks\all_monitor_blocks.h"
    #include "..\workflow_structures.h"
#else
    #include "../../../Monitor-Blocks/all_monitor_blocks.h"
    #include "../workflow_structures.h"
#endif



class WorkflowItem {
    private:
        workflow_item_struct info;
    public:
        WorkflowItem(workflow_item_struct info,MonitorBlock* previous_monitor_block = nullptr);
        ~WorkflowItem(); 

        MonitorBlock* monitor_block;

        bool run_item();
};
