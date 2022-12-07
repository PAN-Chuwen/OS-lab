#ifndef _DEFS_H
#define _DEFS_H

#include "types.h"

#define INF 0xffffffffffffffff

#define PHY_START 0x0000000080000000
#define PHY_SIZE 128 * 1024 * 1024 // 128MB,  QEMU 默认内存大小
#define PHY_END (PHY_START + PHY_SIZE)

#define PGSIZE 0x1000 // 4KB
#define PGROUNDUP(addr) ((addr + PGSIZE - 1) & (~(PGSIZE - 1)))
#define PGROUNDDOWN(addr) (addr & (~(PGSIZE - 1)))

/*
    e.g. uint64 sstatus_value = csr_read(sstatus);
*/
#define csr_read(csr)                 \
    ({                                \
        uint64 __v;                   \
        asm volatile("csrr %0, " #csr \
                     : "=r"(__v)      \
                     :                \
                     : "memory");     \
        __v;                          \
    })

/*
    csr_write macro does not return any value
    e.g. csr_write(sscratch, 0);
*/
#define csr_write(csr, val)              \
    ({                                   \
        uint64 __v = (uint64)(val);      \
        asm volatile("csrw " #csr ", %0" \
                     :                   \
                     : "r"(__v)          \
                     : "memory");        \
    })

#endif
