#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define MAXLINE 9600
#define MAXARGS 28


void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);

int main() {
    char cmdline[MAXLINE];

    while (1) {
        printf("> ");
        fgets(cmdline, MAXLINE, stdin);
        if (feof(stdin)) exit(0);
        printf("processing command %s", cmdline);
        eval(cmdline);
    }
}

// Evaluate a command line
void eval(char *cmdline) {
    char *argv[MAXLINE];
    char buf[MAXLINE];      // holds modified command line
    int bg;                 // the task runs in background or frontground?
    pid_t pid;
    
    strcpy(buf, cmdline);
    bg = parseline(buf, argv);
    if (argv[0] == nullptr) return;

    if (!builtin_command(argv)) {
        if ((pid = fork()) == 0) {     // child process
            if (execve(argv[0], argv, environ) < 0) {
                printf("%s: Command not found\n", argv[0]);
                exit(0);
            }
        }

        if (!bg) {
            int status;
            if (waitpid(pid, &status, 0) < 0) {
                fprintf(stderr, "waitfg: waitpid error");
                exit(0);
            }
        }
        else printf("%d %s", pid, cmdline);
    }
    return;
}

// Parse the command line and build the argv arary
int parseline(char *buf, char **argv) {
    char *delim;
    int argc;
    int bg;

    buf[strlen(buf)-1] = ' ';
    while (*buf && (*buf==' ')) buf++;

    argc = 0;
    while ((delim = strchr(buf, ' '))) {
        argv[argc++] = buf;     // 不用先加'\0'，因为传递的是字符指针，后续输出也是通过此指针读字符串，故只需要后续空格位置字符改为'\0'即可，读取时便到此为止！
        *delim = '\0';      // 将空格改为终止符
        buf = delim + 1;    // 更新buf指针位置开始下一参数检查
        while (*buf && (*buf==' ')) buf++;
    }

    argv[argc] = nullptr;

    if (argc == 0) return 1;

    if ((bg = (*argv[argc-1] == '&')) != 0) argv[--argc] = nullptr;

    return bg;
}

int builtin_command(char **argv) {
    if (!strcmp(argv[0], "quit")) exit(0);
    if (!strcmp(argv[0], "&")) return 1;
    return 0;
}