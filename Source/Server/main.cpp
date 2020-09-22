#include "..\Monitor-Blocks\Command\command.h"
#include <string>
#include "iostream"

int main() {

    std::cout << "Running in main.cpp" << std::endl;
    
    CommandMonitorBlock* block = new CommandMonitorBlock("123","mooshoo","dir");
    block->run_block();

    std::cout << block->print_output() << std::endl;

}


