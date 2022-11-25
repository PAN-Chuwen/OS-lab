.section .text.entry
    .align 2
    .globl _traps 
_traps:

    _save_context:
    # 1. save 32 registers and sepc to stack, because it's a function call 
    # why we need to save epc?
    addi sp, sp, -264
    sd x1, 0(sp)
    sd x2, 8(sp)
   



    # 2. call trap_handler
    j trap_handler

    # 3. restore sepc and 32 registers (x2(sp) should be restore last) from stack
    ld x1, 0(sp)
    ld x2, 8(sp)

    addi sp, sp, 264

    # 4. return from trap
    sret 