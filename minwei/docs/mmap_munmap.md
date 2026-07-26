# mmap/munmap

mmap/munmap底层不维护任何东西，只是返回一个首地址，所分配的内存位于独立的内存映射区（mmap区），不属于堆。

brk/sbrk底层维护一个指针（program break），记录堆的结尾，所分配的内存位于堆中。

brk/sbrk和mmap/munmap是内核提供的两种互相独立的虚拟地址空间管理机制，不存在谁调用谁的关系。malloc等库函数会根据申请内存的大小自行选择：小块内存通常走brk/sbrk（在堆上扩展），大块内存（glibc默认阈值128KB左右）则直接走mmap/munmap。

对于32位机器，每个进程都有4G的虚拟内存空间；64位机器的虚拟地址空间要大得多（如x86-64
Linux通常可用到128TB）。虚拟内存地址只是一个数字，并没有和实际的物理内存相关联。所谓内存分配与释放，起本质就是建立或取消虚拟内存和物理内存的映射关系。

## 创建虚拟内存到物理内存或文件的映射

```c
#include <sys/mman.h>

void *mmap(
    void addr[.length], // 映射区内存起始地址，NULL-系统自动选定，成功返回映射区内存起始地址，失败返回MAP_FAILED（-1）
    size_t length,      // 字节长度，自动按页对齐
    int prot,           // 映射权限
    int flags,          // 映射标志
    int fd,             // 文件描述符，MAP_ANONYMOUS时按惯例传-1
    off_t offset        // 文件偏移量，调用者必须保证是页大小的整数倍，否则返回失败（EINVAL），不会被内核自动对齐
);
```

prot取值：

- `PROT_EXEC`：映射区可执行
- `PROT_READ`：映射区可读
- `PROT_WRITE`：映射区可写
- `PROT_NONE`：映射区不可访问

flags取值：

- `MAP_FIXED`：若在addr上无法创建映射则失败（无此标志系统会自动调整）
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
    void addr[.length], // 映射区内存起始地址，必须是页对齐地址（一般用mmap返回值）
    size_t length // 字节长度，自动按页对齐
);
```

返回值：

- 成功返回0
- 失败返回-1

`addr`和`length`指定的范围可以只是某次mmap映射区的一部分，此时只会取消这部分范围的映射，其余部分仍然有效。


