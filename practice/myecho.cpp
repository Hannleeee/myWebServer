#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {
    printf("Command-line arguments:\n");
    
    for (int i=0; argv[i] != nullptr; ++i)
        printf("    argv[%d]: %s\n", i, argv[i]);
    
    printf("\nEnvironment variables:\n");
    
    for (int i=0; envp[i] != nullptr; ++i)
        printf("    envp[%d]: %s\n", i, envp[i]);

    exit(0);
}