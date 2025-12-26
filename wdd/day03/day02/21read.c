#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

struct student_t 
{
    int no;
    char name[32];
    int score[3];
};

int main() 
{
    int data;
    float num;
    struct student_t s = {};
    int fd = open("student.dat", O_RDONLY);
    if (fd == -1) 
    {
        perror("open");
        return -1;
    }

    ssize_t n = 0;
    n = read(fd, &data, sizeof(data));
    if (n > 0) 
    {
        printf("data = %d\n", data);
    }

    n = read(fd, &num, sizeof(num));
    if (n > 0) 
    {
        printf("num = %f\n", num);
    }

    n = read(fd, &s, sizeof(s));
    if (n > 0) 
    {
        printf("%d %s %d %d %d\n", s.no, s.name, s.score[0], s.score[1], s.score[2]);
    }

    close(fd);

    return 0;
}
