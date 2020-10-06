#include "linux-command_executor.h"

std::tuple<std::string,std::string,int>  execute_commnad(const char* command) {

    // [0] - Child-side of pipe
    // [1] - Parent-side of pipe
    int stdout_pipe[2];
    int stderr_pipe[2];

    pipe(stdout_pipe);
    pipe(stderr_pipe);

    pid_t pID = vfork();
    if(pID == 0){
        dup2(stdout_pipe[0],STDOUT_FILENO);
        dup2(stderr_pipe[0],STDERR_FILENO);

        close(stdout_pipe[0]);
        close(stdout_pipe[1]);
        close(stderr_pipe[0]);
        close(stderr_pipe[1]);

        execl("/bin/bash",command,NULL);
        _exit(0);
    }
    else if (pID <0) {
        return {"","Failed to fork process",-1};
    }
    else {
        int rc {0};
        do {
            wait

        } while (rc != ECHILD);


        char stdout_buffer[100];
        char stderr_buffer[100];
        int stdout_count;
        int stderr_count;

        close(stdout_pipe[0]);
        close(stderr_pipe[0]);

        stdout_count = read(stdout_pipe[1],stdout_buffer,sizeof(stdout_buffer)-1);
        stderr_count = read(stderr_pipe[1],stderr_buffer,sizeof(stderr_buffer)-1);
        
        if (stdout_count >= 0) {
            stdout_buffer[stdout_count] = 0;
            return {stdout_buffer,stderr_buffer,stdout_count};
        }
        else if (stderr_count >= 0)
        {
            stderr_buffer[stderr_count] = 0;
            return {stdout_buffer,stderr_buffer,stderr_count};
        }
        else {
            return {"","Could not create subprocess",-1};
        }
    }
};