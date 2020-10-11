#pragma once

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <tuple>
#include <regex>

namespace inspector {
    std::tuple<std::string,int> execute_command(std::string& command,int& timeout_seconds,const std::string& work_directory);
    std::string simplify_output(const std::string& raw_output,const std::string& language);
};