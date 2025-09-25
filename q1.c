#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int x = 100;
    printf("Before fork, x = %d\n", x);

    pid_t pid = fork();

    if (pid == 0) { // child
        printf("Child: initial x = %d\n", x);
        x = 200;
        printf("Child: changed x = %d\n", x);
    } else { // parent
        printf("Parent: initial x = %d\n", x);
        x = 300;
        printf("Parent: changed x = %d\n", x);
    }
    return 0;
}
