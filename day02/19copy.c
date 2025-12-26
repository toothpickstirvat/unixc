#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int copy(const char* src, const char* des){
    int fd_src = open(src, O_RDONLY);
    if (fd_src == -1){
        printf("open %s failed\n", src);
        return -1;
    }

    int fd_des = open(des, O_WRONLY|O_CREAT|O_EXCL, 0644);
    if (fd_des == -1) {
        if (errno == EEXIST) {
            printf("%s exist, override[yes/no]?", des);
            char check[128] = { 0 };
            scanf("%s", check);
            if (strcasecmp(check, "yes") == 0) 
            {
                fd_des = open(des, O_WRONLY|O_TRUNC);
                if (fd_des == -1) {
                    printf("open %s failed\n", des);
                    return -1; 
                }
            } 
            else 
            { 
                printf("don't override\n");
                return -1;
            }
        } 
        else 
        {
            printf("open %s failed\n", des);
        }
        return -1;
    }
    printf("fd_src=%d, fd_des=%d\n", fd_src, fd_des);
    char data[256] = {0};
    ssize_t n = 0;

    while((n = read(fd_src, data, 10)) > 0) {
        write(fd_des, data, n);
    }

    close(fd_src);
    close(fd_des);

    return 0;
}

int main(int argc, char* argv[]){
    if (argc < 3) {
        printf("%s src_file dest_file\n", argv[0]);
        return -1;
    }
    copy(argv[1], argv[2]);

    return 0;
}