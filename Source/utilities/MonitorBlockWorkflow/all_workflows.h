#pragma once

#ifdef _WIN32
    #include "Item\workflow_item.h"
    #include "Manager\workflows_manager.h"
    #include "Dispatcher\workflow_dispatcher.h"
#else
    #include "Item/workflow_item.h"
    #include "Manager/workflows_manager.h"
    #include "Dispatcher/workflow_dispatcher.h"
#endif