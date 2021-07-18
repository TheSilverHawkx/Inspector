#pragma once

#include "monitor_block.h"

#ifdef _WIN32
    #include "Collector\Command\windows-command_block.h"
    #include "Collector\Script\windows-script_block.h"
    #include "Collector\WMI\windows-wmi_block.h"
    #include "Conditional\SimpleEvaluation\simple-evaluation_block.h"
    //#include "Action\LogToFile\log_to_file_block.h"
#else
    #include "Collector/Command/linux-command_block.h"
    #include "Collector/Script/linux-script_block.h"
    #include "Conditional/SimpleEvaluation/simple-evaluation_block.h"
    //#include "Action/LogToFile/log_to_file_block.h"
#endif