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
    const char* json = "{\"script_language\":\"bash\",\"script_parameters\":\"1 2 3\",\"script_code\":\"#!/bin/bash\\n\\nfor i in `ls`; do\\necho $i\\ndone\"}";

    /*const char* json1 = "{\"script_language\":{\"a\": \"bash\"},\"script_parameters\":\"1 2 3\",\"script_code\":\"#!/bin/bash\\n\\nfor i in `ls`; do\\necho $i\\ndone\"}";
    rapidjson::Document doc;

    doc.Parse(json1);
    std::cout << doc["script_language"].HasMember("a") << std::endl;
    */

    //std::vector<std::string> yeet_list {};
    //yeet_list.push_back("5");

    const char* conditions = "{\"field\": \"equals \"5\"}";
    const char* conditions_text = "{\"condition_operator\":\"equals\",\"condition_value\": \"5\"}";
    rapidjson::Document doc;

    std::vector<std::string> bleep {};
    doc.Parse(conditions_text);

    //% need to add parsing error handling
    if (doc.HasParseError()) {
        std::cout <<  doc.GetParseError();
    }
    else {
    rapidjson::Value& yee = doc;

    if (inspector::evaluate_condition(yee,bleep)){
        std::cout << "true";
    }
    else {
        std::cout << "false";
    }
    }
    //inspector::evaluate_condition(doc,yeet_list);
    // Script Powershell
    //const char* json = "{\"script_language\":\"powershell\",\"script_parameters\":\"1 2 3\",\"script_code\":\"write-host 'yeet1'\\nwrite-host 'yeet2'\"}";
    // Script Batch
    //const char* json = "{\"script_language\":\"batch\",\"script_parameters\":\"1 2 3\",\"script_code\":\"echo bat1\\necho bat2\"}";
    // WMI
    //const char* json = "{\"namespace\":\"root\\\\cimv2\",\"query\":\"select * from win32_service where Name like '%plugplay%'\",\"target\":\"localhost\"}";

    //std::string script = "{\"commandline\":\"/bin/bash -c \\\"echo hello;echo tea\\\"\"}";
    //const char * script = "{\"commandline\":\"sc query pluglay\"}";
    //CommandMonitorBlock* block = new CommandMonitorBlock("123","mooshoo",script.c_str());
    //ScriptMonitorBlock* block = new ScriptMonitorBlock("123","script_block",json);
    //WMIMonitorBlock* block = new WMIMonitorBlock("123","script_block",json);
    //block->run_block();
}


