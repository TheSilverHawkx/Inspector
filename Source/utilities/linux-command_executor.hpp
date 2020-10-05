#ifndef __UTILITY_COMMAND_EXECUTOR__
#define __UTILITY_COMMAND_EXECUTOR__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
// https://github.com/sni/mod_gearman/blob/master/common/popenRWE.c

int execute_commnad(const char* command)
    int out[2];
    int err[2];
    int pid;
    int rc;

    if (_pipe(out) < 0 ) { return -1; }

    if (_pipe(err) < 0 ) {
        close(out[0]);
        close(out[1]);
        return -1;
    }

    pid = _fork
#endif