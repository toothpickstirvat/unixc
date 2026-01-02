#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
extern char** environ;

int main(int argc, char* argv[])
{
    printf("pid: %d\n", getpid());
    printf("cmd line args:\n");
    for(int i = 0; i < argc; i++)
    {
        printf("\t %s\n", argv[i]);
    }

    printf("env vars:\n");
    char** env;
    for (env = environ; *env != NULL; ++env)
    {
        printf("\t %s\n", *env);
    }

    int ret = write(3, "hello,world!", 5);
    printf("[3] ret=%d\n", ret);
    ret = write(4, "hello,world!", 5);
    printf("[4] ret=%d\n", ret);

    return 0;
}
