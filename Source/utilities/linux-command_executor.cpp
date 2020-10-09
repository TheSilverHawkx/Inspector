#include "linux-command_executor.h"

std::tuple<std::string,std::string,int>  execute_commnad(const char* command) {
    int stdout_pipe[2];
    int stderr_pipe[2];

    pid_t child_pid {};
    int status {};
    char buf[1000] {};
    std::string stdout_string, stderr_string {};

    if (pipe(stdout_pipe) == -1){
        perror("out pipe");
        return {"","pipe",EXIT_FAILURE};
    }

    if (pipe(stderr_pipe) == -1){
        close(stdout_pipe[0]);
        close(stdout_pipe[1]);
        perror("err pipe");
        return {"","pipe",EXIT_FAILURE};
    }

    child_pid = fork();
    if (child_pid == -1) {
        close(stdout_pipe[0]);
        close(stderr_pipe[0]);
        close(stdout_pipe[1]);
        close(stderr_pipe[1]);
        return {"","error forking processs",-1};
    }
    else if (child_pid == 0) { // Child
        dup2(stdout_pipe[1],STDOUT_FILENO);
        dup2(stderr_pipe[1],STDERR_FILENO);

        close(stdout_pipe[0]);
        close(stderr_pipe[0]);
        close(stderr_pipe[1]);
        close(stdout_pipe[1]);

        if (system(command) == 0) {
            exit(0);
        }
        else
        {
            exit(errno);
            
        }
        
        
    }
    else {
        close(stdout_pipe[1]);
        close(stderr_pipe[1]);
        do {
            if (waitpid(child_pid,&status,0) == -1){
                return {"","error on waitpid",-1};
            }

            if (WIFEXITED(status)){
                while (read(stdout_pipe[0],&buf,sizeof(buf)))
                {
                    stdout_string.append(buf);
                }
                memset(buf,0,sizeof buf);

                while (read(stderr_pipe[0],&buf,sizeof(buf)))
                {
                    stderr_string.append(buf);
                }
                
                
                close (stdout_pipe[0]);
                close (stderr_pipe[0]);
                printf("exited, status=%d\n",WEXITSTATUS(status));
            }

        } while (!WIFEXITED(status));
        return {stdout_string,stderr_string,status};
    }
};