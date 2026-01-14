#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main()
{
    // fds[0]: read
    // fds[1]: write
    int fds[2];
    int ret = pipe(fds);
    if (ret < 0)
    {
        perror("pipe");
        return -1;
    }
    printf("fds[0]=%d, fds[1]=%d\n", fds[0], fds[1]);
    pid_t pid = fork();
    if (pid <0)
    {
        perror("fork");
        return -1;
    }

    if (pid == 0) 
    {
        // child
        // close write
        close(fds[1]);

        char buf[128] = {0};
        int n = read(fds[0], buf, 128);
        write(STDOUT_FILENO, buf, 128);
    }
    else 
    {
        // parent
        // close read
        close(fds[0]);

        write(fds[1], "hello world\n", 12);
    }


    return 0;
}

