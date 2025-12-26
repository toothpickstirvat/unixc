#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void menu()
{
    printf("id:%d test lock\n", getpid());
    printf("\t1.testlock\n");
    printf("\t2.lock\n");
    printf("\t3.unlock\n");
    printf("\t3.quit\n");
    printf(">>>");
}

void readlock(struct flock* lock)
{
    printf("lock pos(0-SEEK_SET, 1:SEEK_CUR, 2:SEEK_END):");
    int whence = 0;
    scanf("%d", &whence);
    lock->l_whence = 0 == whence ? SEEK_SET : (1 == whence?SEEK_CUR:SEEK_END);
    off_t off = 0;
    printf("offset:");
    scanf("%ld", &off);
    lock->l_start = off;
    unsigned int len = 0;
    printf("len:");
    scanf("%u", &len);
    lock->l_len = len;
    lock->l_pid = getpid(); // -1
}

void showlock(struct flock* lock)
{
    printf("==========================\n");
    printf("type: %s\n", lock->l_type == F_RDLCK ? "read lock" : "write lock");
    printf("pos: %s\n", lock->l_whence == SEEK_SET ? "start" : lock->l_whence == SEEK_CUR ? "cur" : "end");
    printf("offset: %ld\n", lock->l_start);
    printf("len: %ld\n", lock->l_len);
    printf("pid: %d\n", lock->l_pid);
    printf("==========================\n");
}

void trylock(int fd)
{
    struct flock lock = {};
    readlock(&lock);
    printf("test read or write lock?(0-read lock, 1-write lock):");
    int type = 0;
    scanf("%d", &type);
    lock.l_type = type == 0?  F_RDLCK : F_WRLCK;
    int ret = fcntl(fd, F_GETLK, &lock);
    if (ret == -1)
    {
        perror("fcntl");
    }
    if (lock.l_type == F_UNLCK)
    {
        printf("can lock, want to lock?[yes/no]");
        char check[64] = {};
        scanf("%s", check);
        if (strcasecmp(check, "yes") ==0)
        {
            lock.l_type = 0 == type ? F_RDLCK : F_WRLCK;
            ret = fcntl(fd, F_SETLK, &lock);
            if (ret == -1)
            {
                perror("lock failed");
            }
            else
            {
                printf("lock succeed!\n");
            }
        }
    }
    else
    {
        printf("cannot lock!\n");
        showlock(&lock);
    }
}

void lock(int fd)
{
    struct flock lock = {};
    readlock(&lock);
    printf("0-read lock, 1-write lock:");
    int type = 0;
    scanf("%d", &type);
    lock.l_type = type == 0 ? F_RDLCK : F_WRLCK;
    printf("1-wait, 0-nowait:");
    int wait = 0;
    scanf("%d", &wait);
    int ret = fcntl(fd, wait==0?F_SETLK:F_SETLKW,&lock);
    if (ret == -1)
    {
        perror("fcntl lock failed");
    }
    else
    {
        printf("lock succeed!\n");
    }

} 

void unlock(int fd)
{
    struct flock lock = {};
    readlock(&lock);
    lock.l_type = F_UNLCK;
    int ret = fcntl(fd, F_SETLK, &lock);
    if (ret == -1)
    {
        perror("unlock failed");
    }
    else
    {
        printf("unlock succeed!\n");
    }
}

void test()
{
    int fd = open("a.txt", O_RDWR|O_CREAT, 0644);
    if (fd == -1)
    {
        perror("open");
        return;
    }
    while(true)
    {
        menu();
        int in = 0;
        scanf("%d", &in);
        switch(in)
        {
            case 0:
                close(fd);
                return;
            case 1:
                trylock(fd);
                break;
            case 2:
                lock(fd);
                break;
            case 3:
                unlock(fd);
                break;
            default:
                printf("error\n");
        }
    }
}

int main()
{
    test();
    return 0;
}
