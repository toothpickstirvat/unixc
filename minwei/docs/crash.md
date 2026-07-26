# day02/crash.c 分析：为什么越界写没有报错

```c
int* p1 = malloc(sizeof(int));
int* p2 = malloc(sizeof(int));
free(p2);

p1[1] = 0;
p1[2] = 0;
p1[3] = 0;
free(p1);
```

直觉上`p1`只申请了4字节，`p1[1]~p1[3]`已经越界，应该会破坏malloc的内部结构，但实际运行不报错、不崩溃。原因和glibc malloc
chunk的内存布局有关。

## chunk布局

每个chunk在用户指针之前有两个8字节（64位系统）的字段：

```
prev_size (8字节)  // 仅当前一个chunk是空闲状态时才有效，记录前一个chunk大小，用于向前合并
size      (8字节)  // 本chunk的大小 + 标志位（PREV_INUSE等）
                   // ↑ 用户指针(p1)从这里之后开始
```

chunk大小最小为32字节（`MINSIZE`），且必须是16字节的整数倍。`malloc(4)`实际会分配一个32字节的chunk，其中`size`字段占8字节开销，因此
`malloc_usable_size(p1) == 24`（不是4）。

## 关键点：prev_size字段可以被"借用"

`prev_size`字段只有在 **前一个chunk空闲**时才会被读取（用来向前合并）。只要前一个chunk还处于已分配状态，这8字节glibc完全不看，于是干脆把它算进前一个chunk的可用空间里。

以`malloc(4)`为例（chunk大小32，可用24字节），相对`p1`的偏移布局实测如下：

| 偏移（字节） | 对应下标        | 内容                      | 说明                                                          |
|--------------|-----------------|---------------------------|---------------------------------------------------------------|
| 0~15         | `p1[0]`~`p1[3]` | p1自己的数据区            | 正常可用空间                                                  |
| 16~23        | `p1[4]`~`p1[5]` | p2 chunk的`prev_size`字段 | p1未释放时此字段不生效，可安全借用，因此被算进`usable_size`里 |
| 24~31        | `p1[6]`~`p1[7]` | p2 chunk的`size`字段      | **malloc真正用来判断合法性的元数据**（chunk大小+标志位）      |
| 32~          | `p1[8]`起       | p2自己的数据区            | 即`p2`指针本身                                                |

所以：

- `p1[1]~p1[3]`（偏移4~15）落在p1自己的可用区里，本来就没有越界。
- 即使写到`p1[4]`、`p1[5]`（偏移16~23），也只是踩在p2的`prev_size`上，只要p1没被释放，这个字段glibc压根不检查，同样不会出问题。
- 只有写到`p1[6]`及以后，才真正碰到p2 chunk的`size`字段——这才是malloc在`free()`/内存分配时会校验的数据。

## 写得更远也不一定报错：踩进top chunk（p1[16]）

如果写的偏移更大，比如`p1[16]`（偏移64字节），这时候已经完全越过了p2的整个chunk（p2 chunk占偏移16~47共32字节），落到了堆上的**
top chunk（wilderness chunk）**里——而且踩的是top chunk的数据区，不是它的`size`字段（top chunk的`size`字段在其起点偏移+8的地方）。

top chunk本质上是 **还没有分配给任何人的空闲堆内存**，往它中间写0既不影响任何已分配chunk的元数据，也不影响top chunk自己的
`size`字段，所以即使关掉tcache也一样不会报错——因为这次是真的没有破坏任何有意义的数据，不是"检测不到"，而是"没什么可检测的"。

这说明越界写会不会被发现，只取决于有没有踩中某个chunk的`size`字段（或fd/bk等链表指针），偏移越大不代表越危险，也不代表越安全，关键看踩的位置。

## 就算踩到size字段，也不一定报错：tcache的影响

现代glibc（2.26+）默认启用tcache：`free()`小对象时只是简单挂进tcache链表， **不会去检查相邻chunk的size是否合法**。因此就算故意写
`p1[6] = 0`破坏p2的size字段，默认情况下`free(p1)`依然不会报错。

只有关闭tcache，走到老的fastbin合并逻辑（会校验相邻chunk的size是否在合理范围内）时，才能真正触发检测。

### GLIBC_TUNABLES 和 tcache_count 是什么

`GLIBC_TUNABLES`是glibc（2.26+）提供的、不用重新编译程序就能调整运行时参数的环境变量机制，格式是`glibc.<模块>.<参数名>=值`
（多个用逗号分隔）。

tcache是每个线程私有的、按size分bin的单链表缓存，专门存放刚`free()`掉的小块内存，下次`malloc`
同样大小时直接摘一个返回，不用加锁也不用做复杂的合并/校验，所以更快。

`glibc.malloc.tcache_count`控制 **每个size对应的bin最多能缓存多少个已释放的chunk**，默认是7：

- bin未满时，`free()`直接把chunk挂进tcache链表，不检查相邻chunk的size——这就是`p1[6]=0`默认不报错的原因。
- 设成`tcache_count=0`，相当于每个bin容量为0、永远是"满的"，于是每次`free()`都跳过tcache，直接走会做完整校验的老路径（fastbin/unsorted
  bin），之前被tcache掩盖的损坏就会暴露出来：

```bash
GLIBC_TUNABLES=glibc.malloc.tcache_count=0 ./crash
# free(): invalid next size (fast)
# Aborted (core dumped)
```

### 另一种不用改运行方式、只改代码就能复现的办法

tcache只接管"小块"分配（默认上限约1032字节）。把分配大小改到超过这个阈值（比如`malloc(2000)`），`free()`
就会天然绕开tcache、走会做完整校验的路径，不需要设置任何环境变量：

```c
int* p1 = malloc(2000);
int* p2 = malloc(2000);
free(p2);

size_t idx = malloc_usable_size(p1) / sizeof(int); // 正好踩到p2 chunk的size字段
p1[idx] = 0;
free(p1);
```

实测会直接报`double free or corruption (!prev)`并`Aborted`。

## 结论

1. 堆溢出是否会被malloc发现，取决于到底写没写到malloc真正依赖的元数据（`size`
   字段等），而不是取决于是否超过了"申请时"的大小、也不是偏移量大小——因为chunk按最小粒度对齐会有天然富余空间，越过相邻chunk之后还可能落进top
   chunk这种"本来就空闲"的区域，怎么写都不算破坏。
2.
就算写到了元数据，glibc默认的tcache快速路径也基本不做校验，corruption可能潜伏很久都不报错，直到某次操作（consolidation、fastbin/smallbin校验等）恰好触发检查才会crash。这也是堆溢出类漏洞难以发现、危害性大的根本原因——不能依赖"程序不崩溃"来判断内存是否安全，需要借助ASan、valgrind等工具主动检测。
3. 想稳定复现报错，两种办法：a) 用`GLIBC_TUNABLES=glibc.malloc.tcache_count=0`关闭tcache后再运行；b) 把分配大小改到超过tcache上限，让
   `free()`天然绕开tcache。
