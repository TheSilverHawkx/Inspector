#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <tuple>
#include <sys/wait.h>

std::tuple<std::string,std::string,int>  execute_commnad(const char* command);		
				