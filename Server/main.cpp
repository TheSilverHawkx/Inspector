#include "../Monitor Blocks\monitor_block.h"
#include <string>
#include "iostream"

int main() {

    std::cout << "hello" << std::endl;
    
    MonitorBlock* block = new MonitorBlock("123","mooshoo",_block_type::trigger,"1",_output_type::JSON);


}


