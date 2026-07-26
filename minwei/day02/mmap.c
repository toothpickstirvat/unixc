#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>


#define MAX_TEXT 256

int main() {
    char* psz = (char*)mmap(
        NULL, 
        MAX_TEXT * sizeof(char),
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        0,
        0
    );

    if (psz == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    sprintf(psz, "Hello, World!");
    printf("%s\n", psz);
    printf("psz = %p\n", psz);
    printf("查看/proc/%u/maps，按回车退出...", getpid());
    getchar();

    if (munmap(psz, MAX_TEXT*sizeof(char)) == -1)
    {
        perror("munmap");
        return -1;
    }

    return 0;
}
