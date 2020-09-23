#ifdef _WIN32
    #include "..\Monitor-Blocks\Command\command_block.h"
#else
    #include "../Monitor-Blocks/Command/command_block.h"
#endif
#include <string>
#include "iostream"

int main() {

    std::cout << "Running in main.cpp" << std::endl;
    
    CommandMonitorBlock* block = new CommandMonitorBlock("123","mooshoo","dir");
    block->run_block();

    std::cout << block->print_output() << std::endl;

}


