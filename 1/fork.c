#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main() {
    int status;
    int pid = fork();
// Write C code here   
    if (pid == 0) {
        int pid2 = getpid();
        int pid3 = fork();
        if (pid3 == 0) {
            int pid4 = getpid();
            printf("I am the grandchild (%d)\n", pid4);
        }
        else if (pid3 > 0) {
            wait(NULL);
            printf("I am the child (%d)\n", pid2); /* LINE X */
        }
    }
    else if (pid > 0) {
        wait(NULL);
        int pid5 = getpid();
        printf("I am the parent (%d)\n", pid5);
    }
    return 0;
}