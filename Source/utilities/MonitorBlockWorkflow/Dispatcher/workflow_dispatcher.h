#pragma once

#include <chrono>
#include <ctime>
#include <fstream>
#include <vector>
#include <thread>

#ifdef _WIN32
    #include "..\Manager\workflows_manager.h"
#else
    #include "../Manager/workflows_manager.h"
#endif

struct dispatcher_entry {
    unsigned int trigger_id;
    unsigned int trigger_type;
    std::string parameter;
    unsigned int workflow_id;
};

class WorkflowDispatcher {
    private:
        const char* workflow_list_path;
        std::vector<dispatcher_entry> workflows_table {};

        void create_workflow(dispatcher_entry* entry);
        void start();

    public:
        WorkflowDispatcher(const char* path);
        ~WorkflowDispatcher(); 

        void update_table();
};
