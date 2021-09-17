#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t pid;
    pid = fork();
    if (pid==0) {
        sleep(30);
        exit(0);
    }
    printf("Process: %d\n", getpid());
    printf("Child process: %d\n", pid);
    sleep(20);
    exit(0);

}