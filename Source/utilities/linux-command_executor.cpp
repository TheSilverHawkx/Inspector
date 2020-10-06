#include "linux-command_executor.h"

std::tuple<std::string,std::string,int>  execute_commnad(const char* command) {

    // [0] - Child-side of pipe
    // [1] - Parent-side of pipe
    int stdout_pipe[2];
    int stderr_pipe[2];

    if (pipe(stdout_pipe))
    {
        return {"","Could not open stdout pipe",-1};
    }

    if (pipe(stderr_pipe)) {
        close(stdout_pipe[0]);
        close(stdout_pipe[1]);
        return {"","Could not open stderr pipe", -1};
    }

    pid_t pID = fork();
    switch (pID){
        case -1: // Error while forking
            close(stdout_pipe[0]);
            close(stdout_pipe[1]);
            close(stderr_pipe[0]);
            close(stderr_pipe[1]);
            return {"","Could not fork process", -1};

        case 0: // Child Process
            close(stdout_pipe[0]);
            close(stderr_pipe[0]);

            dup2(stdout_pipe[1],1);
            dup2(stderr_pipe[1],2);
            execl("/bin/bash",command,NULL);
            return {"","",-1};

        default: // Parent Process
            close(stdout_pipe[1]);
            close(stderr_pipe[1]);

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

        /*int rc {0};
        do {
            wait

        } while (rc != ECHILD);*/
};