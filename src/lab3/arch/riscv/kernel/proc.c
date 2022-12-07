#include "proc.h"
#include "printk.h"
#include "mm.h"
#include "rand.h"
#include "defs.h"

extern void __dummy();

struct task_struct* idle;           // idle process
struct task_struct* current;        // 指向当前运行线程的 `task_struct`
struct task_struct* task[NR_TASKS]; // 线程数组, 所有的线程都保存在此

void task_init() {
    // 1. 调用 kalloc() 为 idle 分配一个物理页
    idle = (struct task_struct*) kalloc();
    // 2. 设置 state 为 TASK_RUNNING;
    idle->state = TASK_RUNNING;
    // 3. 由于 idle 不参与调度 可以将其 counter / priority 设置为 0
    idle->counter = 0;
    idle->priority = 0;
    // 4. 设置 idle 的 pid 为 0
    idle->pid = 0;
    // 5. 将 current 和 task[0] 指向 idle
    current = idle;
    task[0] = idle;
    // note that struct member thread of idle should not be used

    uint64 currentPID;
    for (currentPID = 1; currentPID < NR_TASKS; currentPID++) {
    // 1. 参考 idle 的设置, 为 task[1] ~ task[NR_TASKS - 1] 进行初始化
        task[currentPID] = (struct task_struct*) kalloc();

    // 2. 其中每个线程的 state 为 TASK_RUNNING, counter 为 0, priority 使用 rand() 来设置, pid 为该线程在线程数组中的下标。
        task[currentPID]->state = TASK_RUNNING;
        task[currentPID]->counter = 0;
        task[currentPID]->priority = rand();
        task[currentPID]->pid = currentPID;

    // 3. 为 task[1] ~ task[NR_TASKS - 1] 设置 `thread_struct` 中的 `ra` 和 `sp`,
    // 4. 其中 `ra` 设置为 __dummy （见 4.3.2）的地址,  `sp` 设置为 该线程申请的物理页的高地址
        task[currentPID]->thread.ra = (uint64) __dummy;
        task[currentPID]->thread.sp = (uint64) task[currentPID] + PGSIZE;
    }
   


    printk("...proc_init done!\n");
}


void dummy() {
    uint64 MOD = 1000000007;
    uint64 auto_inc_local_var = 0;
    int last_counter = -1; // first time switch to this process
    while(1) {
        if (current->counter != last_counter) {
            last_counter = current->counter;
            auto_inc_local_var = (auto_inc_local_var + 1) % MOD;
            printk("[PID = %d] is running. auto_inc_local_var = %d\n", current->pid, auto_inc_local_var);
        }
    }
}


void do_timer() {
    if (current == idle) {
        printk("switch to task1\n");
        switch_to(task[1]);
    } else {
        current->counter--;
        if (current->counter <= 0) {
            schedule();
        }
        else {
            // do nothing
        }
    }
}

void schedule() {

}

// in entry.S, linker will do the work
extern void __switch_to(struct task_struct* prev, struct task_struct* next); 

void switch_to(struct task_struct* next) {
    // jump to __switch_to if the next process scheduled is not the same as current one
    if (current != next) {
        struct task_struct* old = current;
        current = next;
        __switch_to(old, next);
    }
    // ret is NOT called at the end of this function if switch happends (when first time switch to the new task)
    // when >= 2 times, task execute from this line
}