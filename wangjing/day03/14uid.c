#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

int main()
{
    printf("real UID: %d\n", getuid());
    printf("effecitve UID: %d\n", geteuid());
    struct passwd* p = getpwuid(getuid());
    printf("username: %s, password: %s\n", p->pw_name, p->pw_passwd);

    printf("real GID: %d\n", getgid());
    printf("effecitve GID: %d\n", getegid());   
    struct group* g = getgrgid(getgid());
    printf("group name: %s, group password: %s\n", g->gr_name, g->gr_passwd);
    return 0;
}


