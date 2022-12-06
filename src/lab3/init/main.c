#include "printk.h"
#include "sbi.h"

extern void test();

int start_kernel() {
    test(); // DO NOT DELETE !!!

	return 0;
}
