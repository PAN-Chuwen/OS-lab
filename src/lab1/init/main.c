#include "print.h"
#include "sbi.h"

extern void test();

int start_kernel() {
    sbi_ecall(0x1, 0x0, 0x33, 0, 0, 0, 0, 0); 
    puti(2022);
    puts(" Hello RISC-V\n");
    test(); // DO NOT DELETE !!!

	return 0;
}
