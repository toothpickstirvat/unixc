#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>


char* mode(mode_t m)
{
    static char s[128] = {};
    if (S_ISREG(m))
    {
        s[0] = '-';
    }
    else if(S_ISDIR(m))
    {
        s[0] = 'd';
    }
    else if(S_ISLNK(m))
    {
        s[0] = 'l';
    }
    else if(S_ISFIFO(m))
    {
        s[0] = 'p';
    }
    else if(S_ISBLK(m))
    {
        s[0] = 'b';
    }
    else if(S_ISCHR(m))
    {
        s[0] = 'c';
    }
    else if(S_ISSOCK(m))
    {
        s[0] = 's';
    }
    else
    {
        s[0] = '-';
    }
    /*
    strcat(s, m & S_IRUSR ? "r" : "-");
    strcat(s, m & S_IWUSR ? "w" : "-");
    strcat(s, m & S_IXUSR ? "x" : "-");

    strcat(s, m & S_IRGRP ? "r" : "-");
    strcat(s, m & S_IWGRP ? "w" : "-");
    strcat(s, m & S_IXGRP ? "x" : "-");

    strcat(s, m & S_IROTH ? "r" : "-");
    strcat(s, m & S_IWOTH ? "w" : "-");
    strcat(s, m & S_IXOTH ? "x" : "-");
    */

    s[1] = m & S_IRUSR ? 'r' : '-';
    s[2] = m & S_IWUSR ? 'w' : '-';
    s[3] = m & S_IXUSR ? 'x' : '-';
    if (m & S_ISUID) // 如果有设置用户ID
    {
        s[3] = s[3] == 'x' ? 's' : 'S';
    }
    s[4] = m & S_IRGRP ? 'r' : '-';
    s[5] = m & S_IWGRP ? 'w' : '-';
    s[6] = m & S_IXGRP ? 'x' : '-';
    if (m & S_ISGID) // 如果有设置组ID
    {
        s[6] = s[6] == 'x' ? 's' : 'S';
    }
    s[7] = m & S_IROTH ? 'r' : '-';
    s[8] = m & S_IWOTH ? 'w' : '-';
    s[9] = m & S_IXOTH ? 'x' : '-';
    if (m & __S_ISVTX) // 如果有粘滞位
    {
        s[9] = s[9] == 'x' ? 't' : 'T';
    }
    return s;
}

void list(const char* file)
{
    struct stat s = {};
    int ret = stat(file, &s);
    if (ret == -1)
    {
        perror("stat");
        return;
    }

    char* m = mode(s.st_mode);
    printf("%s %lu %d %d %lu %s\n", m, s.st_nlink, s.st_uid, s.st_gid, s.st_size, ctime(&s.st_mtime));

}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("%s file\n", argv[0]);
        return -1;
    }

    list(argv[1]);

    return 0;
}
