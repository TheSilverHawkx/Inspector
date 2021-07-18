#pragma once

#include "monitor_block.h"

#ifdef _WIN32
    #include "Conditional\SimpleEvaluation\simple-evaluation_block.h"
#else
    #include "Conditional/SimpleEvaluation/simple-evaluation_block.h"
#endif