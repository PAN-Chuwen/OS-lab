.extern start_kernel

    .section .text.init
    .globl _start
_start:

    # set stvec = _traps
    la t0, _traps
    csrw stvec, t0

    # set sie[STIE] = 1, enable Time Interrupt
    addi t0, zero, 32
    # cannot use csrrsi here since the uimm field only supports 5 bits
    csrrs x0, sie, t0


    # set first time interrupt
    addi a7, zero, 0x00 # extension ID
    mv a6, zero # function ID
    mv a1, zero
    mv a2, zero
    mv a3, zero
    mv a4, zero
    mv a5, zero
    
    li t1, 1000000
    rdtime t0;
    add a0, t0, t1 # a0 stores the time for next interrupt to happen
    ecall  # ecall stores a0 -> mtimecmp in M mode


    # set sstatus[SIE] = 1, enable Interrupts in S mode
    csrrsi x0, sstatus, 0x2

    la sp, boot_stack
    j start_kernel
    .section .bss.stack
    .globl boot_stack
boot_stack:
    .space 4096 #stack size = 4KB

    .globl boot_stack_top
boot_stack_top: