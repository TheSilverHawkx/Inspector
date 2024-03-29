#pragma once

#include <chrono>
#include <ctime>
#include <fstream>
#include <vector>
#include <thread>
#include <map>


#ifdef _WIN32
    #include "..\Manager\workflows_manager.h"
    #include "..\..\DBConnector\Agent\db_connector.h"
    #include "..\workflow_structures.h"
#else
    #include "../Manager/workflows_manager.h"
    #include "../workflow_structures.h"
    #include "../../DBConnector/Agent/db_connector.h"
#endif



class WorkflowDispatcher {
    private:
        DBConnector* db_con;
        const char* workflow_list_path;
        std::vector<dispatcher_entry> workflows_table {};

        void create_workflow(dispatcher_entry* entry);
        void start();

    public:
        bool is_running {true};
        WorkflowDispatcher(const char* path, DBConnector* connector);
        ~WorkflowDispatcher(); 

        void update_table();

        // Temporary
        void add_entry(dispatcher_entry& entry);
};
