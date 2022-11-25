.extern start_kernel

    .section .text.init
    .globl _start
_start:

    # set stvec = _traps
    la t0, _traps
    csrw stvec, t0

    # set sie[STIE] = 1, enable Time Interrupt
    csrrs x0, sie, 0x10


    # set first time interrupt
    


    # set sstatus[SIE] = 1, enable Interrupts in S mode
    csrrs x0, sstatus, 0x2

    la sp, boot_stack
    j start_kernel
    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096 #stack size = 4KB

    .globl boot_stack_top
boot_stack_top: