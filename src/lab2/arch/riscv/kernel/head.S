.extern start_kernel

    .section .text.init
    .globl _start
_start:
    la sp, boot_stack
    j start_kernel
    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096 #stack size = 4KB

    .globl boot_stack_top
boot_stack_top: