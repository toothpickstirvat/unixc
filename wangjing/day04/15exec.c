#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


int main()
{
    pid_t pid = vfork();
    if (pid == -1)
    {
        perror("vfork");
        return -1;
    }
    if (pid == 0)
    {
        //execl("/usr/bin/ls", "~", NULL);
        //execlp("ls", "~", NULL);

        char* argvs[] = {"~", NULL};
        //execv("/usr/bin/ls", argvs);
        execvp("ls", argvs);
    }
    else
    {
        printf("[Parent] pid: %d\n", getpid());
    }

    return 0;
}
