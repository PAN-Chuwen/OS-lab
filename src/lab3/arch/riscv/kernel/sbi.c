#include "types.h"
#include "sbi.h"


struct sbiret sbi_ecall(int ext, int fid, uint64 arg0,
			            uint64 arg1, uint64 arg2,
			            uint64 arg3, uint64 arg4,
			            uint64 arg5) 
{
	struct sbiret ret_struct;
	uint64 ext_u64 = ext;
	uint64 fid_u64 = fid;
	/*
		将 ext (Extension ID) 放入寄存器 a7 中
		fid (Function ID) 放入寄存器 a6 中
		将 arg0 ~ arg5 放入寄存器 a0 ~ a5 中
		OpenSBI 的返回结果会存放在寄存器 a0, a1 中，
		其中 a0 为 error code， a1 为返回值， 我们用 sbiret 来接受这两个返回值。
	*/
    __asm__ volatile (
        "mv x17, %[ext]\n"
        "mv x16, %[fid]\n"
		"mv x10, %[arg0]\n"
		"mv x11, %[arg1]\n"
		"mv x12, %[arg2]\n"
		"mv x13, %[arg3]\n"
		"mv x14, %[arg4]\n"
		"mv x15, %[arg5]\n"
		"ecall\n"
		"mv %[error_code], x10\n"
		"mv %[ret_val], x11"
        : [error_code] "=r" (ret_struct.error), [ret_val] "=r" (ret_struct.value)
        : [ext] "r" (ext_u64), [fid] "r" (fid_u64), \
		  [arg0] "r" (arg0), [arg1] "r" (arg1), [arg2] "r" (arg2), [arg3] "r" (arg3), [arg4] "r" (arg4), [arg5] "r" (arg5) 
        : "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"
    );
    return ret_struct;     
}
