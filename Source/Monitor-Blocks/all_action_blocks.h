#pragma once

#include "monitor_block.h"

#ifdef _WIN32
    #include "Action\LogToFile\log_to_file_block.h"
#else
    #include "Action/LogToFile/log_to_file_block.h"
#endif