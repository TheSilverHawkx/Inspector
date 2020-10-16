#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <tuple>
#include <sys/wait.h>

namespace inspector {
    std::tuple<std::string,std::string,int>  execute_command(const char* command);		
}
				