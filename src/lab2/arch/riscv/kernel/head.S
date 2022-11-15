.extern start_kernel

    .section .text.init
    .globl _start
_start:

    # set stvec = _traps



    # set sie[STIE] = 1



    # set first time interrupt



    # set sstatus[SIE] = 1


    la sp, boot_stack
    j start_kernel
    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096 #stack size = 4KB

    .globl boot_stack_top
boot_stack_top: