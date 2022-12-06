#ifndef _DEFS_H
#define _DEFS_H

#include "types.h"
/*
    e.g. uint64 sstatus_value = csr_read(sstatus);
*/
#define csr_read(csr)                       \
({                                          \
    uint64 __v;                             \
    asm volatile ("csrr %0, " #csr      \
                    : "=r" (__v)             \
                    :                       \
                    : "memory");            \
    __v;                                    \
})

/*
    csr_write macro does not return any value
    e.g. csr_write(sscratch, 0);
*/
#define csr_write(csr, val)                         \
({                                                  \
    uint64 __v = (uint64)(val);                     \
    asm volatile ("csrw " #csr ", %0"               \
                    : : "r" (__v)                   \
                    : "memory");                    \
})

#endif
