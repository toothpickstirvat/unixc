#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int* p1 = malloc(sizeof(int));
    int* p2 = malloc(sizeof(int));
    printf("p1 = %p, p2 = %p\n", p1, p2);
    free(p2);

    p1[6] = 0;
    free(p1);

    printf("end\n");
    return 0;
}

// GLIBC_TUNABLES=glibc.malloc.tcache_count=0 ./a.out

