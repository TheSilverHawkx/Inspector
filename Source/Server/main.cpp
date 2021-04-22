#ifdef _WIN32
    #include "..\Monitor-Blocks\all_monitor_blocks.h"
    #include "..\..\include\rapidjson\document.h"
    #include "..\..\include\rapidjson\writer.h"
    #include "..\utilities\condition_parser.h"
    #include "..\Utilities\MonitorBlockWorkflow\all_workflows.h"
    #include <cstdio>
    #include "..\Utilities\DBConnector\Agent\db_connector.h"
#else
    #include "../../include/rapidjson/document.h"
    #include "../Monitor-Blocks/all_monitor_blocks.h"
    #include "../Utilities/MonitorBlockWorkflow/all_workflows.h"
    #include "../Utilities/DBConnector/Agent/db_connector.h"

#endif

#include <stdexcept>
#include <string>
#include "iostream"
#include <filesystem>
#include <vector>

#include <sstream>

namespace fs = std::filesystem;


int main() {
    // Initialize Work Folder
    const std::string work_directory_path = fs::current_path().string();
    fs::create_directory("workdir");
    fs::permissions("workdir",fs::perms::others_all, fs::perm_options::remove);

    // Initialize DB
    DBConnector* db_con = new DBConnector((fs::current_path() / "workdir\\agent.db").string().c_str());

    std::cout << "Running in main.cpp" << std::endl;
    // Playground

    
    WorkflowDispatcher* dispatcher = new WorkflowDispatcher((fs::current_path() / "workdir\\workflows.csv").string().c_str(),db_con);


    //db_con->set("{\"script_language\":\"bash\",\"script_parameters\":\"1 2 3\",\"script_code\":\"#!/bin/bash\\n\\nfor i in `ls`; do\\necho $i\\ndone\"}");

/*
    dispatcher_time_struct time {1,2,3,4,5,6};
    dispatcher_entry kaki {1,2,time,3};
    dispatcher->add_entry(kaki);*/
}


