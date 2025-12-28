#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

void printdir(const char* path, int depth)
{
    DIR* dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir");
        return;
    }
    struct dirent* pd = NULL;
    while(pd = readdir(dir))
    {
        if (pd->d_name[0] == '.')
        {
            continue;
        }
        
        printf("%*s%s\n", depth * 4, " ", pd->d_name);
        if (pd->d_type == DT_DIR)
        {
            if (!(strcmp(pd->d_name, ".") == 0 || strcmp(pd->d_name, "..") == 0))
            {
                char pathname[256] = {};
                strcpy(pathname, path);
                strcat(pathname, "/");
                strcat(pathname, pd->d_name);
                printdir(pathname, depth + 1);
            }
        }
        
        
    }

    closedir(dir);
}

void treedir(const char* path)
{
    printf("%s\n", path);
    printdir(path, 1);

}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        treedir(".");
    }
    else
    {
        treedir(argv[1]);
    }

    return 0;
}