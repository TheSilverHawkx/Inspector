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


int main() {

    std::cout << "Running in main.cpp" << std::endl;
    std::string script = "#!/bin/bash \necho 'hello'";
    const char* json = "{\"script_language\":\"python3\",\"script_parameters\":\"1 2 3\",\"script_code\":\"import sys\nprint(sys.argv[2])\"}";
    //CommandMonitorBlock* block = new CommandMonitorBlock("123","mooshoo",script.c_str());
    ScriptMonitorBlock* block = new ScriptMonitorBlock("123","script_block",json);
    block->run_block();

    std::cout << block->print_output() << std::endl;

}


