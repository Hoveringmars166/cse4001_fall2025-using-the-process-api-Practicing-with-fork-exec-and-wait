#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        printf("Child running\n");
        return 42;
    } else {
        int status;
        pid_t w = wait(&status);
        printf("Parent: child pid %d ended with exit code %d\n",
               w, WEXITSTATUS(status));
    }
    return 0;
}
