#include "printk.h"
#include "defs.h"

// Please do not modify

void test() {

    csr_write(sscratch, 0x13);
    uint64 sstatus_value = csr_read(sstatus);
    
    while (1);
}
