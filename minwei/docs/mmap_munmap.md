# mmap/munmap

mmap/munmap底层不维护任何东西，只是返回一个首地址，所分配的内存位于堆中。

brk/sbrk底层维护一个指针，记录所分配的内存结尾，所分配的内存位于堆中，底层调用mmap/munmap。

对于32位机器，每个进程都有4G的虚拟内存空间，虚拟内存地址只是一个数字，并没有和实际的物理内存相关联。所谓内存分配与释放，起本质就是建立或取消虚拟内存和物理内存的映射关系。


## 创建虚拟内存到物理内存或文件的映射

```c
#include <sys/mman.h>

void *mmap(
    void addr[.length], // 映射区内存起始地址，NULL-系统自动选定，成功返回映射区内存起始地址，失败返回MAP_FAILED（-1）
    size_t length,      // 字节长度，自动按页对齐
    int prot,           // 映射权限
    int flags,          // 映射标志
    int fd,             // 文件描述符
    off_t offset        // 文件偏移量，自动按页对齐
);
```

prot取值：

- `PROT_EXEC`：映射区可执行
- `PROT_READ`：映射区可读
- `PROT_WRITE`：映射区可写
- `PROT_NONE`：映射区不可访问

flags取值：

- `MAP_FIXED`：若再addr上无法创建映射则失败（无此标志系统会自动调整）
- `MAP_SHARED`：对映射区的写入操作直接反映到文件中
- `MAP_PRIVATE`：对映射区的写入操作只反映到缓冲区中，不会写入文件
- `MAP_ANONYMOUS`：匿名映射，将虚拟地址映射到物理内存而非文件，忽略fd
- `MAP_DENYWRITE`：拒绝其他对文件的写入操作
- `MAP_LOCKED`：锁定映射区，保证其不被置换

返回值：

- 成功返回指向映射区的指针
- 失败返回MAP_FAILED（-1）

## 销毁虚拟内存到物理内存或文件的映射

```c
#include <sys/mman.h>

int munmap(
    void addr[.length], // 映射区内存起始地址
    size_t length // 字节长度，自动按页对齐
);
```

返回值：

- 成功返回0
- 失败返回-1


