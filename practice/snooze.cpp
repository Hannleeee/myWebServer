#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void handler(int sig) { return; }

unsigned int snooze(unsigned int secs) {
    unsigned int rest = sleep(secs);
    printf("%d of %d", secs-rest, secs);
    return rest;
}

int main(int argc, char *argv[]) {
    if (signal(SIGINT, handler) == SIG_ERR) {
        fprintf(stderr, "signal error\n");
        exit(1);
    }

    unsigned int t = 0;
    char *p = argv[1];
    while (*p != NULL) {
        t = 10 * t + (unsigned int)(*p - '0');
        p++;
    }
    snooze(t);
    return 0;
}