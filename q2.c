#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    int fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    pid_t pid = fork();

    if (pid == 0) { // child
        write(fd, "Child writing\n", 14);
    } else { // parent
        write(fd, "Parent writing\n", 15);
    }

    close(fd);
    return 0;
}
