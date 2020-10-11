#pragma once

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <tuple>

std::tuple<std::string,int> execute_commnad(std::string& command,int& timeout_seconds);