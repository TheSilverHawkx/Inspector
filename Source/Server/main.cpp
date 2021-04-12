#ifdef _WIN32
    #include "..\Monitor-Blocks\Collector\Command\windows-command_block.h"
    #include "..\Monitor-Blocks\Collector\Script\windows-script_block.h"
    #include "..\Monitor-Blocks\Collector\WMI\windows-wmi_block.h"
    #include "..\..\include\rapidjson\document.h"
    #include "..\utilities\condition_parser.h"
    #include "..\..\include\rapidjson\filereadstream.h"
    #include <cstdio>
#else
    #include "../../include/rapidjson/document.h"
    #include "../Monitor-Blocks/Collector/Command/linux-command_block.h"
    #include "../Monitor-Blocks/Collector/Script/linux-script_block.h"
#endif

#include "..\Monitor-Blocks\Conditional\SimpleEvaluation\simple-evaluation_block.h"

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
    
    // Script Bash
    //const char* json = "{\"script_language\":\"bash\",\"script_parameters\":\"1 2 3\",\"script_code\":\"#!/bin/bash\\n\\nfor i in `ls`; do\\necho $i\\ndone\"}";
    //Script Powershell
    //const char* json = "{\"script_language\":\"powershell\",\"script_parameters\":\"1 2 3\",\"script_code\":\" Get-Content 'C:\\\\Users\\\\omerr\\\\OneDrive\\\\Desktop\\\\yeet1.txt'\"}";
    // Script Batch
    //const char* json = "{\"script_language\":\"batch\",\"script_parameters\":\"1 2 3\",\"script_code\":\"echo bat1\\necho bat2\"}";
    // WMI
    //const char* json = "{\"namespace\":\"root\\\\cimv2\",\"query\":\"select * from win32_service where Name like '%plugplay%'\",\"target\":\"localhost\"}";
    // Command Windows
    const char * command = "{\"commandline\":\"sc query plugplay\"}";
    // Command Linux
    //const char * command = "{\"commandline\":\"/bin/bash -c \"echo hello\"}";

    CommandMonitorBlock* collector_block = new CommandMonitorBlock("123","mooshoo",command);
    //ScriptMonitorBlock* collector_block = new ScriptMonitorBlock("123","script_block",json);
    //WMIMonitorBlock* collector_block = new WMIMonitorBlock("123","script_block",json);
    collector_block->run_block();

    // Mock data for Conditions
    // Simple Condition
    //const char* mock_condition = "{\"condition_operator\":\"contains\",\"condition_value\": \"Video\",\"index\" : 0}";
    // AND Condition
    //const char* mock_condition = "{\"group_operator\" : \"and\", \"conditions\": [{\"condition_operator\":\"contains\",\"condition_value\": \"Video\",\"index\" : 0},{\"condition_operator\":\"contains\",\"condition_value\": \"due\",\"index\" : 0}]}";
    // OR Condition
    const char* mock_condition = "{\"group_operator\" : \"or\", \"conditions\": [{\"condition_operator\":\"contains\",\"condition_value\": \"video\",\"index\" : 0},{\"condition_operator\":\"contains\",\"condition_value\": \"Video\",\"index\" : 0}]}";
    // NESTED Condition
    //const char* mock_condition = "{\"group_operator\" : \"and\", \"conditions\": [{\"condition_operator\":\"equals\",\"condition_value\": \"4\",\"index\" : 0},{\"group_operator\" : \"or\", \"conditions\": [{\"condition_operator\":\"equals\",\"condition_value\": \"5\",\"index\" : 0},{\"condition_operator\":\"equals\",\"condition_value\": \"6\",\"index\" : 0}]}]}";

    SimpleEvaluationBlock* conditional_block = new SimpleEvaluationBlock("123",mock_condition,collector_block);
    conditional_block->run_block();

    if (conditional_block->evaluation_status) {
        std::cout << "All good!";
    }
    else {
        std::cout << "bad!";
    }
}


