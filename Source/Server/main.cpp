#ifdef _WIN32
    #include "..\Monitor-Blocks\all_collector_blocks.h"
    #include "..\Monitor-Blocks\all_condition_blocks.h"
    #include "..\Monitor-Blocks\all_action_blocks.h"
    #include "..\..\include\rapidjson\document.h"
    #include "..\..\include\rapidjson\writer.h"
    #include "..\utilities\condition_parser.h"
    #include "..\Utilities\MonitorBlockWorkflow\all_workflows.h"
    #include <cstdio>
    #include "..\Utilities\DBConnector\Agent\db_connector.h"
    #include "..\Utilities\Exceptions.h"
#else
    #include "../../include/rapidjson/document.h"
    #include "../Monitor-Blocks/all_monitor_blocks.h"
    #include "../utilities/MonitorBlockWorkflow/all_workflows.h"
    #include "../utilities/DBConnector/Agent/db_connector.h"
    #include "../utilities/Exceptions.h"

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

    /*WorkflowDispatcher* dispatcher = new WorkflowDispatcher((fs::current_path() / "workdir\\workflows.csv").string().c_str(),db_con);

    while (dispatcher->is_running){
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }*/

    ScriptMonitorBlock* script_mb = new ScriptMonitorBlock("1","one","{\"script_language\":\"powershell\",\"script_parameters\":\"1 2 3\",\"script_code\":\" Get-Content 'C:\\\\Users\\\\omerr\\\\OneDrive\\\\Desktop\\\\yeet1.txt'\"}");
    script_mb->run_block();
    script_mb->block_type = _block_type::collector;

    LogToFileBlock* ltf_mb = new LogToFileBlock("2","{\"file_path\": \"\",\"mode\": 1,\"text\": {\"template\": \"random text with [var-1] spaces a[var-2]nd stuff\",\"custom_parts\": [{\"name\": \"1\", \"value\": 0},{\"name\": \"2\", \"value\": 1}]}}",script_mb);

    ltf_mb->run_block();

    /* Serialize dispatcher_time_struct to db
    dispatcher_time_struct a {10,999,999,999,999,999};
    std::stringstream kuki;
    a.serialize(kuki);

    dispatcher_time_struct bb;

    bb.deserialize(kuki);

    std::vector<char> shuki;
    std::string& shuki2 = kuki.str();
    
    for (int i = 0; i < shuki2.size(); i++)
    {
        shuki.push_back(shuki2[i]);
    }
    
    db_con->add_dispatch_entry("one","mock_workflow",shuki);
    */
}


