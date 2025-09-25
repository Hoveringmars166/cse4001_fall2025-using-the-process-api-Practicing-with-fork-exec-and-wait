#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        sleep(1);
        printf("Child done\n");
        return 0;
    } else {
        int status;
        waitpid(pid, &status, 0);
        printf("Parent waited for child pid %d (exit code %d)\n",
               pid, WEXITSTATUS(status));
    }
    return 0;
}
