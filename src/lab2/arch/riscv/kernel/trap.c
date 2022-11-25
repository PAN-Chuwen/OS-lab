// trap.c 
#include "clock.h"
#include "types.h"
#include "printk.h"

const int MACHINE_TIMER_INTERRUPT_CODE = 7;

void trap_handler(uint64 scause, uint64 sepc) {
    // 通过 `scause` 判断trap类型
    // 如果是interrupt 判断是否是timer interrupt
    // 如果是timer interrupt 则打印输出相关信息, 并通过 `clock_set_next_event()` 设置下一次时钟中断
    // `clock_set_next_event()` 见 4.5 节
    // 其他interrupt / exception 可以直接忽略

    if ((scause >> 63) > 0) { // highest bit of scause = 1 => Interrupt
        uint32 interruptCode = scause; // get lower 32 bits of scause
        if (interruptCode == MACHINE_TIMER_INTERRUPT_CODE) {
            printk("[S] Supervisor Mode Timer Interrupt\n");
            clock_set_next_event();
        }
    }
}