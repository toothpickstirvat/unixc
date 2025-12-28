#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("%s dir\n", argv[0]);
        return -1;
    }

    int ret = chdir(argv[1]);
    if (ret == -1)
    {
        perror("chdir");
        return -1;
    }
    
    char path[128] = {};
    char* cwd = getcwd(path, 128);
    printf("%s\n", path);

    return 0;
}
