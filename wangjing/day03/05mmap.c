#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>

int copy_mmap(const char* src_file, const char* dest_file)
{
    int fd_src = open(src_file, O_RDONLY);  // 只读打开
    if (fd_src == -1) {
        perror("open src file");
        return -1;
    }
    
    // 获取文件大小
    struct stat st;
    if (fstat(fd_src, &st) == -1) {
        perror("fstat");
        close(fd_src);
        return -1;
    }
    off_t filesize = st.st_size;
    
    // 空文件直接返回
    if (filesize == 0) {
        close(fd_src);
        creat(dest_file, 0644);
        return 0;
    }
    
    // 创建目标文件并设置大小
    int fd_dest = open(dest_file, O_RDWR|O_CREAT|O_TRUNC, 0644);
    if (fd_dest == -1) {
        perror("open dest file");
        close(fd_src);
        return -1;
    }
    
    // 预分配磁盘空间（避免ftruncate失败）
    if (ftruncate(fd_dest, filesize) == -1) {
        perror("ftruncate");
        close(fd_src);
        close(fd_dest);
        return -1;
    }
    
    // 映射源文件（只读）
    void* psrc = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd_src, 0);
    if (psrc == MAP_FAILED) {
        perror("mmap src");
        close(fd_src);
        close(fd_dest);
        return -1;
    }
    
    // 映射目标文件（读写）
    void* pdest = mmap(NULL, filesize, PROT_READ|PROT_WRITE, MAP_SHARED, fd_dest, 0);
    if (pdest == MAP_FAILED) {
        perror("mmap dest");
        munmap(psrc, filesize);
        close(fd_src);
        close(fd_dest);
        return -1;
    }
    
    // 拷贝数据
    memcpy(pdest, psrc, filesize);
    
    // 确保数据写入磁盘
    msync(pdest, filesize, MS_SYNC);
    
    // 清理
    munmap(psrc, filesize);
    munmap(pdest, filesize);
    close(fd_src);
    close(fd_dest);
    
    return 0;
}



int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("%s src_file dest_file\n", argv[0]);
        return -1;
    }

    copy(argv[1], argv[2]);


    return 0;
}