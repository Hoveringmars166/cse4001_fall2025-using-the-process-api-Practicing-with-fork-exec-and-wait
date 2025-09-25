#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ; // needed for execle/execvpe

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        char *args[] = {"ls", "-l", NULL};
        execvp("ls", args);

        perror("exec failed");
    } else {
        wait(NULL);
    }
    return 0;
}
