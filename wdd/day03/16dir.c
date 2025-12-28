#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>


int main(int argc, char* argv[])
{
    DIR* dir = opendir(".");
    if (dir == NULL)
    {
        perror("opendir");
        return -1;
    }

    struct dirent* pd = NULL;
    while(pd = readdir(dir))
    {
        printf("%s\n", pd->d_name);
    }

    closedir(dir);

    return 0;
}