#pragma once

#include <string>

#ifdef _WIN32
    #include "..\Item\workflow_item.h"
    #include "..\workflow_structures.h"
#else
    #include "../Item/workflow_item.h"
    #include "../workflow_structures.h"
#endif

class WorkflowManager {
    private:
        const char* workflow_id;
        std::map<unsigned int,workflow_item_struct>workflow_items;

    public:
        WorkflowManager(const char* id,std::map<unsigned int,workflow_item_struct> items);
        ~WorkflowManager(); 

        void run_workflow();
};
