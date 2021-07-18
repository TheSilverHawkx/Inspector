#pragma once

#include <string>

#ifdef _WIN32
    #include "..\..\..\Monitor-Blocks\all_collector_blocks.h"
    #include "..\..\..\Monitor-Blocks\all_condition_blocks.h"
    #include "..\..\..\Monitor-Blocks\all_action_blocks.h"
    #include "..\workflow_structures.h"
#else
    #include "../../../Monitor-Blocks/all_collector_blocks.h"
    #include "../../../Monitor-Blocks/all_condition_blocks.h"
    #include "../../../Monitor-Blocks/all_action_blocks.h"
    #include "../workflow_structures.h"
#endif



class WorkflowItem {
    private:
        workflow_item_struct info;
    public:
        WorkflowItem(workflow_item_struct info,WorkflowItem* previous_workflow_item = nullptr);
        ~WorkflowItem(); 

        MonitorBlock* monitor_block;

        bool run_item();
};
