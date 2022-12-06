#include "sbi.h"
#include "types.h"
// clock.c

// QEMU中时钟的频率是10MHz, 也就是1秒钟相当于10000000个时钟周期。
uint64 TIMECLOCK = 10000000;

uint64 get_cycles() {
    // 编写内联汇编，使用 rdtime 获取 time 寄存器中 (也就是mtime 寄存器 )的值并返回
    uint64 currentTime;
    __asm__ volatile (
        
        "rdtime %[currentTime]"
        : [currentTime] "=r" (currentTime) \
        : 
        : "memory"
    );
    return currentTime;
}

uint64 clock_set_next_event() {
    // 下一次 时钟中断 的时间点
    uint64 next = get_cycles() + TIMECLOCK;

    // 使用 sbi_ecall 来完成对下一次时钟中断的设置
    sbi_ecall(0, 0, next, 0, 0, 0, 0, 0);
} 