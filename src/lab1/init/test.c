#include "print.h"
#include "defs.h"

// Please do not modify

void test() {
    puts("write 0x0 to sscratch using csr_write\n");
    csr_write(sscratch, 0x13);
    puts("read sstatus using csr_read\n");
    uint64 sstatus_value = csr_read(sstatus);
    
    while (1);
}
