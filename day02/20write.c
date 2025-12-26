#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

struct student_t {
    int no;
    char name[32];
    int score[3];
};

int main() 
{
    int data = 1024;
    float num = 6.19;
    struct student_t s = {2023001, "Alice", {100, 98, 89}};
    int fd = open("student.dat", O_WRONLY|O_CREAT|O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    ssize_t n = 0;
    if (write(fd, &data, sizeof(data)) < sizeof(data)){
        printf("write data error\n");
    }

    n = write(fd, &num, sizeof(num));
    n = write(fd, &s, sizeof(s));
    

    close(fd);

    return 0;
}
