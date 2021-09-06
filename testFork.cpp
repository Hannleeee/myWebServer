#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <errno.h>
#include <string.h>

int main() {
    int status, i;
    pid_t pid;

    for (int i = 0; i < 3; ++i)
        if ((pid = fork()) == 0)
            exit(100+i);

    while ((pid = waitpid(-1, &status, 0)) > 0) {
        if (WIFEXITED(status)) {
            printf("child process %d terminated normally with exit status = %d\n", pid, WEXITSTATUS(status));
        }
        else printf("child process %d terminated abnormally\n", pid);
    }

    if (errno != ECHILD) {
        fprintf(stderr, "waitpid error: %s\n", strerror(errno));
        exit(0);
    }
}