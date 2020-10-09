#ifdef _WIN32
    #include "..\Monitor-Blocks\Collector\Command\windows-command_block.h"
    #include "..\Monitor-Blocks\Collector\Script\windows-script_block.h"
    #include "..\Monitor-Blocks\Collector\WMI\windows-wmi_block.h"
    #include "..\..\include\rapidjson\document.h"
    #include "..\..\include\rapidjson\filereadstream.h"
    #include <cstdio>
    #include <stdexcept>
#else
    #include "../../include/rapidjson/document.h"
    #include "../Monitor-Blocks/Collector/Command/linux-command_block.h"
    #include "../Monitor-Blocks/Collector/Script/linux-script_block.h"
#endif

#include <string>
#include <fstream>
#include "iostream"
#include <filesystem>
#include <tuple>

namespace fs = std::filesystem;


int main() {
    const std::string work_directory_path = fs::current_path().string();
    fs::create_directory("workdir");
    fs::permissions("workdir",fs::perms::others_all, fs::perm_options::remove);

    std::cout << "Running in main.cpp" << std::endl;
    
    // Script Bash
    //const char* json = "{\"script_language\":\"bash\",\"script_parameters\":\"1 2 3\",\"script_code\":\"#!/bin/bash\\n\\nfor i in `ls`; do\\necho $i\\ndone\"}";
    // Script Powershell
    //const char* json = "{\"script_language\":\"powershell\",\"script_parameters\":\"1 2 3\",\"script_code\":\"write-host 'yeet1'\\nwrite-host 'yeet2'\"}";
    // Script Batch
    //const char* json = "{\"script_language\":\"batch\",\"script_parameters\":\"1 2 3\",\"script_code\":\"echo bat1\\necho bat2\"}";
    // WMI
    //const char* json = "{\"namespace\":\"root\\\\cimv2\",\"query\":\"select * from win32_service where Name like '%plugplay%'\",\"target\":\"localhost\"}";

    std::string script = "/bin/bash -c \"echo hello\"";
    CommandMonitorBlock* block = new CommandMonitorBlock("123","mooshoo",script.c_str());
    //ScriptMonitorBlock* block = new ScriptMonitorBlock("123","script_block",json);
    //WMIMonitorBlock* block = new WMIMonitorBlock("123","script_block",json);
    block->run_block();
}


