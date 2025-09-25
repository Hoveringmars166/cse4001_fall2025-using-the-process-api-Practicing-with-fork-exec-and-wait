# Assignment: Practicing the Process API
Practicing with fork, exec, wait. 

### Overview

In this assignment, you will practice using the Process API to create processes and run programs under Linux. The goal is to gain hands-on experience with system calls related to process management. Specifically, you will practice using the unix process API functions 'fork()', 'exec()', 'wait()', and 'exit()'. 

⚠️ Note: This is not an OS/161 assignment. You will complete it directly on Linux. 

Use the Linux in your CSE4001 container. If you are using macOS, you may use the Terminal (you may need to install development tools with C/C++ compilers). 

**Reference Reading**: Arpaci-Dusseau, *Operating Systems: Three Easy Pieces*, Chapter 5 (Process API Basics)
 👉 [Chapter 5 PDF](http://pages.cs.wisc.edu/~remzi/OSTEP/cpu-api.pdf)

---

### **Steps to Complete the Assignment**

1. **Accept the GitHub Classroom Invitation**
    [GitHub Link](https://classroom.github.com/a/FZh4BrQG)
2. **Set up your Repository**
   - Clone the assignment repository.
3. **Study the Reference Materials**
   - Read **Chapter 5**.
   - Download and explore the sample programs from the textbook repository:
      [OSTEP CPU API Code](https://github.com/remzi-arpacidusseau/ostep-code/tree/master/cpu-api).
4. **Write Your Programs**
   - Adapt the provided example code to answer the assignment questions.
   - Each program should be clear, well-commented, and compile/run correctly.
   - Add your solution source code to the repository.

5. **Prepare Your Report**
   - Answer the questions in the README.md file. You must edit the README.md file and not create another file with the answers. 
   - For each question:
     - Include your **code**.
     - Provide your **answer/explanation**.
6. **Submit Your Work via GitHub**
   - Push both your **program code** to your assignment repository.
   - This push will serve as your submission.
   - Make sure all files, answers, and screenshots are uploaded and rendered properly.








---
### Questions
1. Write a program that calls `fork()`. Before calling `fork()`, have the main process access a variable (e.g., x) and set its value to something (e.g., 100). What value is the variable in the child process? What happens to the variable when both the child and parent change the value of x?


```cpp
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
```
The variable in the child process initially has the same value as in the parent at the time of the fork(), but when the child or parent changes their copy of the variable, the changes are independent and do not affect each other.


2. Write a program that opens a file (with the `open()` system call) and then calls `fork()` to create a new process. Can both the child and parent access the file descriptor returned by `open()`? What happens when they are writing to the file concurrently, i.e., at the same time?

```cpp
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
```
Both the child and parent inherit and can access the same file descriptor after fork(), and if they write to the file concurrently, their outputs may interleave or appear in an unexpected order because they share the same file offset.

3. Write another program using `fork()`.The child process should print “hello”; the parent process should print “goodbye”. You should try to ensure that the child process always prints first; can you do this without calling `wait()` in the parent?

```cpp
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
```
You can make the child likely to print first by delaying the parent (for example, with sleep()), but without waiting, there is no strict guarantee since the OS scheduler ultimately decides which process runs first.

4. Write a program that calls `fork()` and then calls some form of `exec()` to run the program `/bin/ls`. See if you can try all of the variants of `exec()`, including (on Linux) `execl()`, `execle()`, `execlp()`, `execv()`, `execvp()`, and `execvpe()`. Why do you think there are so many variants of the same basic call?

```cpp
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
```

5. Now write a program that uses `wait()` to wait for the child process to finish in the parent. What does `wait()` return? What happens if you use `wait()` in the child?

```cpp
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
```
wait() returns the PID of the terminated child and stores its exit status in a variable; if called in the child, it fails because the child has no children to wait for.

6. Write a slight modification of the previous program, this time using `waitpid()` instead of `wait()`. When would `waitpid()` be useful?

```cpp
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
```
waitpid() is useful when a parent needs to wait for a specific child to finish, or when it wants to check a child’s status without blocking, such as when managing multiple child processes.
7. Write a program that creates a child process, and then in the child closes standard output (`STDOUT FILENO`). What happens if the child calls `printf()` to print some output after closing the descriptor?

```cpp
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        close(STDOUT_FILENO);
        printf("This will not be printed!\n");
    } else {
        printf("Parent still has stdout.\n");
    }
    return 0;
}
```
If the child closes its standard output (STDOUT_FILENO) and then calls printf(), nothing is printed because the file descriptor no longer exists.
