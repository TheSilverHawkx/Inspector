#include "linux-command_executor.h"

std::tuple<std::string,std::string,int>  execute_commnad(const char* command) {
    int pipefd[2];
    int stdout_pipe[2];
    int stderr_pipe[2];

    pid_t child_pid {};
    int status {};
    char buf;
    
    if (pipe(pipefd) == -1){
        perror("pipe");
        return {"","pipe",EXIT_FAILURE};
    }
    if (pipe(stdout_pipe) == -1){
        perror("out pipe");
        return {"","pipe",EXIT_FAILURE};
    }

    if (pipe(stderr_pipe) == -1){
        perror("err pipe");
        return {"","pipe",EXIT_FAILURE};
    }

    child_pid = fork();
    if (child_pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        close(stdout_pipe[0]);
        close(stdout_pipe[0]);
        return {"","error forking processs",-1};
    }
    else if (child_pid == 0) { // Child
        close(pipefd[0]); // Close unsed write end -> // Close unused read end

        
        write(pipefd[1], "yeey",1);
        write(pipefd[1], "\n",1);
        close(pipefd[1]);

        _exit(0);
    }
    else {
        close(pipefd[1]); // Close unsed read end
        do {
            if (waitpid(child_pid,&status,0) == -1){
                return {"","error on waitpid",-1};
            }

            if (WIFEXITED(status)){
                while (read(pipefd[0],&buf,1) >0){
                    write(STDOUT_FILENO,&buf,1);
                }
                close(pipefd[0]); // Reader will get EOF
                printf("exited, status=%d\n",WEXITSTATUS(status));
            }

        } while (!WIFEXITED(status));
        return {"exited good","",0};
    }


/////*
    // [0] - read
    // [1] - write
    

    /*if (pipe(stdout_pipe))
    {
        return {"","Could not open stdout pipe",-1};
    }

    /*if (pipe(stderr_pipe)) {
        close(stdout_pipe[0]);
        close(stdout_pipe[1]);
        return {"","Could not open stderr pipe", -1};
    }

    pid_t pid = fork();
    switch (pid){
        case -1: // Error while forking
            //close(stdout_pipe[0]);
            //close(stdout_pipe[1]);
            //close(stderr_pipe[0]);
            //close(stderr_pipe[1]);
            return {"","Could not fork process", -1};

        case 0: // Child Process
            //dup2(stdout_pipe[1],STDOUT_FILENO);
            //close(stdout_pipe[0]);
            //close(stdout_pipe[1]);
            
            //close(stderr_pipe[0]);


            //dup2(stderr_pipe[1],2);
            execl("/bin/bash",command,NULL);
            exit(0);

        default: // Parent Process
            //close(stdout_pipe[1]);
            //close(stderr_pipe[1]);

            /*char stdout_buffer[100];
            char stderr_buffer[100];
            int stdout_count;
            int stderr_count;


            int status;
            wait(&status);
            //stdout_count = read(stdout_pipe[1],stdout_buffer,sizeof(stdout_buffer)-1);
            //stderr_count = read(stderr_pipe[1],stderr_buffer,sizeof(stderr_buffer)-1);
            
            //close(stdout_pipe[0]);
            //close(stderr_pipe[0]);
            /*
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