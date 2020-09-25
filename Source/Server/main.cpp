#ifdef _WIN32
    #include "..\Monitor-Blocks\Command\command_block.h"
    #include "..\Monitor-Blocks\Script\script_block.h"
    #include "..\..\include\rapidjson\document.h"
#else
    #include "../../include/rapidjson/document.h"
    #include "../Monitor-Blocks/Command/command_block.h"
    #include "../Monitor-Blocks/Script/script_block.h"
#endif

#include <string>
#include "iostream"
#include <filesystem>

namespace fs = std::filesystem;


int main() {
    const std::string work_directory_path = fs::current_path().string();
    fs::create_directory("workdir");
    fs::permissions("workdir",fs::perms::others_all, fs::perm_options::remove);

    std::cout << "Running in main.cpp" << std::endl;
    std::string script = "#!/bin/bash \necho 'hello'";
    
    const char* json = "{\"script_language\":\"bash\",\"script_parameters\":\"1 2 3\",\"script_code\":\"#!/bin/bash\\n\\nfor i in `ls`; do\\necho $i\\ndone\"}";
    //CommandMonitorBlock* block = new CommandMonitorBlock("123","mooshoo",script.c_str());
    ScriptMonitorBlock* block = new ScriptMonitorBlock("123","script_block",json);
    block->run_block();

    std::cout << block->print_output() << std::endl;

}


