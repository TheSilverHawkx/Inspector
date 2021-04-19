#ifdef _WIN32
    #include "..\Monitor-Blocks\all_monitor_blocks.h"
    #include "..\..\include\rapidjson\document.h"
    #include "..\utilities\condition_parser.h"
    #include "..\Utilities\MonitorBlockWorkflow\all_workflows.h"
    #include <cstdio>
#else
    #include "../../include/rapidjson/document.h"
    #include "../Monitor-Blocks/all_monitor_blocks.h"
    #include "../Utilities/MonitorBlockWorkflow/all_workflows.h"
#endif

#include <stdexcept>
#include <string>
#include "iostream"
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;


int main() {
    const std::string work_directory_path = fs::current_path().string();
    fs::create_directory("workdir");
    fs::permissions("workdir",fs::perms::others_all, fs::perm_options::remove);

    std::cout << "Running in main.cpp" << std::endl;

    
    WorkflowDispatcher* dispatcher = new WorkflowDispatcher((fs::current_path() / "workdir\\workflows.csv").string().c_str());

    
}


