#ifdef _WIN32
    #include "..\Monitor-Blocks\Command\windows-command_block.h"
    #include "..\Monitor-Blocks\Script\windows-script_block.h"
    #include "..\Monitor-Blocks\WMI\windows-wmi_block.h"
    #include "..\..\include\rapidjson\document.h"
    #include "..\..\include\rapidjson\filereadstream.h"
    #include <cstdio>
    #include <stdexcept>
#else
    #include "../../include/rapidjson/document.h"
    #include "../Monitor-Blocks/Command/linux-command_block.h"
    #include "../Monitor-Blocks/Script/linux-script_block.h"
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
    std::string script = "#!/bin/bash \necho 'hello'";
    
    //CommandMonitorBlock* block = new CommandMonitorBlock("123","mooshoo",script.c_str());
    //const char* json = "{\"script_language\":\"bash\",\"script_parameters\":\"1 2 3\",\"script_code\":\"#!/bin/bash\\n\\nfor i in `ls`; do\\necho $i\\ndone\"}";
    //ScriptMonitorBlock* block = new ScriptMonitorBlock("123","script_block",json);
    //\\\\root\\cimvs2
    const char* json = "{\"namespace\":\"root\\\\cimv2\",\"query\":\"select Name,Description from win32_service\",\"target\":\"localhost\"}";
    WMIMonitorBlock* block = new WMIMonitorBlock("123","script_block",json);
    block->run_block();
    //const char* json = "{\"script_language\":\"powershell\",\"script_parameters\":\"1 2 3\",\"script_code\":\"write-host 'yeet1'\\nwrite-host 'yeet2'\"}";
    //std::string script = "#!/bin/bash \necho 'hello'";
    //CommandMonitorBlock* block = new CommandMonitorBlock("123","mooshoo",script.c_str());


    std::cout << "Batch 2 line output" << std::endl;
    const char* json_b_2 = "{\"script_language\":\"batch\",\"script_parameters\":\"1 2 3\",\"script_code\":\"echo bat1\\necho bat2\"}";
    ScriptMonitorBlock* block = new ScriptMonitorBlock("123","script_block",json_b_2);
    block->run_block();
    std::cout << block->print_output() << std::endl;
    delete block;
    
    _sleep(5000);
    std::cout << "Powershell 2 line output" << std::endl;
    const char* json_ps_2 = "{\"script_language\":\"powershell\",\"script_parameters\":\"1 2 3\",\"script_code\":\"write-host ps1\\nwrite-host ps2\"}";
    block = new ScriptMonitorBlock("123","script_block",json_ps_2);
    block->run_block();
    std::cout << block->print_output() << std::endl;
    delete block;

    std::ofstream file;
        file.open("C:\\temp\\yeet.csv");
        file << block->print_output() << std::endl;
        file.close();

}


