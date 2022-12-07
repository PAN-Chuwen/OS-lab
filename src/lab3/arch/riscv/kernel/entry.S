#define TASK_THREAD_RA 40 # 5 * 8 bytes = 40 bytes offset

.section .text.entry
.align 2

# entry for traps
.globl _traps 
_traps:
    _save_context:
    # 1. save 32 registers and sepc to stack
    # why we need to save epc?
    addi sp, sp, -248
    sd x0, 0(sp)
    sd x1, 8(sp)
    sd x2, 16(sp)
    sd x3, 24(sp)
    sd x4, 32(sp)
    sd x5, 40(sp)
    sd x6, 48(sp)
    sd x7, 56(sp)
    sd x8, 64(sp)
    sd x9, 72(sp)
    sd x10, 80(sp)
    sd x11, 88(sp)
    sd x12, 96(sp)
    sd x13, 104(sp)
    sd x14, 112(sp)
    sd x15, 120(sp)
    sd x16, 128(sp)
    sd x17, 136(sp)
    sd x18, 144(sp)
    sd x19, 152(sp)
    sd x20, 160(sp)
    sd x21, 168(sp)
    sd x22, 176(sp)
    sd x23, 184(sp)
    sd x24, 192(sp)
    sd x25, 200(sp)
    sd x26, 208(sp)
    sd x27, 216(sp)
    sd x28, 224(sp)
    sd x29, 232(sp)
    sd x30, 240(sp)

    # 2. call trap_handler
    csrr a0, scause
    csrr a1, sepc
    jal trap_handler

    _restore_context:
    # 3. restore sepc and 32 registers (x2(sp) should be restore last) from stack
    ld x0, 0(sp)
    ld x1, 8(sp)
    ld x2, 16(sp)
    ld x3, 24(sp)
    ld x4, 32(sp)
    ld x5, 40(sp)
    ld x6, 48(sp)
    ld x7, 56(sp)
    ld x8, 64(sp)
    ld x9, 72(sp)
    ld x10, 80(sp)
    ld x11, 88(sp)
    ld x12, 96(sp)
    ld x13, 104(sp)
    ld x14, 112(sp)
    ld x15, 120(sp)
    ld x16, 128(sp)
    ld x17, 136(sp)
    ld x18, 144(sp)
    ld x19, 152(sp)
    ld x20, 160(sp)
    ld x21, 168(sp)
    ld x22, 176(sp)
    ld x23, 184(sp)
    ld x24, 192(sp)
    ld x25, 200(sp)
    ld x26, 208(sp)
    ld x27, 216(sp)
    ld x28, 224(sp)
    ld x29, 232(sp)
    ld x30, 240(sp)
    addi sp, sp, 248

    # 4. return from trap
    sret 

.global __dummy
__dummy:
    la t0, dummy
    la ra, _restore_context
    csrw sepc, t0
    # return from trap
    sret

# when entering __switch_to, a0 contains previous PCB address, a1 contains next PCB address to switch to 
.globl __switch_to
__switch_to:
    # get address of ra for both tasks (old & new)
    li a4, TASK_THREAD_RA
    add a3, a0, a4  # a3 now points to old task's thread_struct
    add a4, a1, a4  # a4 now points to new task's thread_struct

    # save state(ra, sp, s0-s11) to prev process
    sd ra, 0(a3)
    sd sp, 8(a3)
    # sd s0, 16(a3)
    # sd s1, 24(a3)
    # sd s2, 32(a3)
    # sd s3, 40(a3)
    # sd s4, 48(a3)
    # sd s5, 56(a3)
    # sd s6, 64(a3)
    # sd s7, 72(a3)
    # sd s8, 80(a3)
    # sd s9, 88(a3)
    # sd s10, 96(a3)
    # sd s11, 104(a3)

    # restore state from next process
    ld ra, 0(a4)
    ld sp, 8(a4)
    # ld s0, 16(a4)
    # ld s1, 24(a4)
    # ld s2, 32(a4)
    # ld s3, 40(a4)
    # ld s4, 48(a4)
    # ld s5, 56(a4)
    # ld s6, 64(a4)
    # ld s7, 72(a4)
    # ld s8, 80(a4)
    # ld s9, 88(a4)
    # ld s10, 96(a4)
    # ld s11, 104(a4)

    ret # PC <- $ra, will jump to __dummy or dummy()