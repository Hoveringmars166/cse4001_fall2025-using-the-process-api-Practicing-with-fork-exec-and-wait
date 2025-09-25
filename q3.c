#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        printf("hello\n");
    } else {
        sleep(1); // give child time
        printf("goodbye\n");
    }
    return 0;
}
