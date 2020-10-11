#include "windows-command_executor.h"

std::tuple<std::string,int> inspector::execute_command(std::string& command,int& timeout_seconds,const std::string& work_directory) {
    HANDLE parent_stdout_handle = nullptr;
    HANDLE child_stdout_handle = nullptr;
    STARTUPINFO si = {sizeof(STARTUPINFO)};
    PROCESS_INFORMATION pi;
    DWORD return_code = 0;
    std::string subprocess_output {};

    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Connect Child stdout to parent_stdout_handle
    if (!CreatePipe(&parent_stdout_handle,&child_stdout_handle,&saAttr,0))
    {
        subprocess_output = "Could not create stdout pipe. Error: " + std::to_string(GetLastError());
        CloseHandle(child_stdout_handle);
        CloseHandle(parent_stdout_handle);
        
        return {subprocess_output,-1};
    }

    if (! SetHandleInformation(parent_stdout_handle,HANDLE_FLAG_INHERIT,0))
    {
        subprocess_output = "Could not set handle information for stdout pipe. Error: " + std::to_string(GetLastError());
        CloseHandle(child_stdout_handle);
        CloseHandle(parent_stdout_handle);
        return {subprocess_output,-1};
    }

    si.cb = sizeof(si);
    si.hStdOutput = child_stdout_handle;
    si.hStdError = child_stdout_handle;
    si.dwFlags |= STARTF_USESTDHANDLES;

    // Create Process
    if (!CreateProcess(NULL,LPSTR(command.c_str()),NULL,NULL,TRUE,0,NULL,LPCSTR(work_directory.c_str()),&si,&pi)) {
        subprocess_output = "Failed to create process. Error: " + std::to_string(GetLastError());
        CloseHandle(child_stdout_handle);
        CloseHandle(parent_stdout_handle);

        return {subprocess_output,-1};
    }
    CloseHandle(child_stdout_handle);

    // Wait for process to finish or timeout
    if (timeout_seconds == NULL) {
        WaitForSingleObject(pi.hProcess,INFINITE);
    }
    else {
        WaitForSingleObject(pi.hProcess,DWORD(timeout_seconds * 1000));
    }
    
    
    if (STILL_ACTIVE == return_code) {
        TerminateProcess(pi.hProcess,return_code);
        subprocess_output = "Command did not finish within defined timeout threshold";
    }
    else {
        DWORD dwRead = 1L;
        CHAR chBuf[4096] = "";
        BOOL bSuccess = FALSE;

        for (;;) {
            bSuccess = ReadFile(parent_stdout_handle,chBuf,4096,&dwRead,NULL);
            if (! bSuccess || dwRead == 0) break;
            subprocess_output = chBuf;
        }
    }

    if (!GetExitCodeProcess(pi.hProcess,&return_code)) {
        subprocess_output = "Failed to fetch exit code";
        CloseHandle(parent_stdout_handle);
        return {subprocess_output,-1};
    }

    CloseHandle(parent_stdout_handle);

    return {subprocess_output,return_code};
};

std::string inspector::simplify_output(const std::string& raw_output,const std::string& language) {
    std::string parsed_output {};
    if (language == "batch") {
        std::string script_name = raw_output.substr(raw_output.find("\r\n",0)+2,raw_output.find(">",0)-1);

        std::regex delimiter{"\r\n"};
        std::vector<std::string> lines {
            std::sregex_token_iterator(raw_output.begin(),raw_output.end(),delimiter,-1), {}
        };

        for (auto& line:lines) {
            if (!line.empty() & std::string::npos == line.find(script_name,0))
            {
                parsed_output += line + "\n";
            }
        }
                
        return parsed_output.substr(0,parsed_output.length()-1) ;
    }
    else if (language == "powershell") {
        return raw_output.substr(0,raw_output.length()-1);
    }
    else {
        return "";
    }
};