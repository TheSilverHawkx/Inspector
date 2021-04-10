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
    const char* json = "{\"script_language\":\"powershell\",\"script_parameters\":\"1 2 3\",\"script_code\":\"write-host 'yeet1'\\nwrite-host 'yeet2'\"}";
    // Script Batch
    //const char* json = "{\"script_language\":\"batch\",\"script_parameters\":\"1 2 3\",\"script_code\":\"echo bat1\\necho bat2\"}";
    // WMI
    //const char* json = "{\"namespace\":\"root\\\\cimv2\",\"query\":\"select * from win32_service where Name like '%plugplay%'\",\"target\":\"localhost\"}";

    //std::string script = "{\"commandline\":\"/bin/bash -c \\\"echo hello;echo tea\\\"\"}";
    //const char * script = "{\"commandline\":\"sc query pluglay\"}";
    //CommandMonitorBlock* block = new CommandMonitorBlock("123","mooshoo",script.c_str());
    ScriptMonitorBlock* collector_block = new ScriptMonitorBlock("123","script_block",json);
    //WMIMonitorBlock* block = new WMIMonitorBlock("123","script_block",json);
    collector_block->run_block();

    // Mock data for Conditions
    std::vector<std::string> bleep {"yeet1"};
    //const char* simple_conditions = "{\"condition_operator\":\"equals\",\"condition_value\": \"shoop\",\"index\" : 0}";
    //const char* and_conditions = "{\"group_operator\" : \"and\", \"conditions\": [{\"condition_operator\":\"equals\",\"condition_value\": \"5\",\"index\" : 0},{\"condition_operator\":\"equals\",\"condition_value\": \"4\",\"index\" : 0}]}";
    //const char* or_conditions = "{\"group_operator\" : \"or\", \"conditions\": [{\"condition_operator\":\"equals\",\"condition_value\": \"5\",\"index\" : 0},{\"condition_operator\":\"equals\",\"condition_value\": \"4\",\"index\" : 0}]}";
    const char* nested_conditions = "{\"group_operator\" : \"and\", \"conditions\": [{\"condition_operator\":\"equals\",\"condition_value\": \"4\",\"index\" : 0},{\"group_operator\" : \"or\", \"conditions\": [{\"condition_operator\":\"equals\",\"condition_value\": \"5\",\"index\" : 0},{\"condition_operator\":\"equals\",\"condition_value\": \"6\",\"index\" : 0}]}]}";

    //% need to add parsing error handling
}


