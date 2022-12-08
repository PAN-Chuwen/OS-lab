#include "proc.h"
#include "printk.h"
#include "mm.h"
#include "rand.h"
#include "defs.h"

extern void __dummy();

struct task_struct* idle;           // idle process
struct task_struct* current;        // 指向当前运行线程的 `task_struct`
struct task_struct* task[NR_TASKS]; // 线程数组, 所有的线程都保存在此
uint64 shortestPID = 1;
uint64 currentPID;
uint64 minCount = INF;
uint64 zeroCount = 0;

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
        if (last_counter == -1 || current->counter != last_counter) {
            last_counter = current->counter;
            auto_inc_local_var = (auto_inc_local_var + 1) % MOD;
            printk("[PID = %d] is running. counter = %d\n", current->pid, current->counter);
        }
    }
}


void do_timer() {
    if (current == idle) {
        schedule();
    } else {
        if (current->counter > 0) {
            current->counter--;
        } else {
            schedule();
        }
    }
}

void schedule() {
    
    /* all variables used in schedule should NOT be declared local 
    *  local = stored in task's own stack (every task has one minCount if doing so)
    *  global(see top of proc.c) = stored in OS's stack
    */ 
    for (currentPID = 1, zeroCount = 0, shortestPID = 1, minCount = INF; currentPID < NR_TASKS; currentPID++) {
        if (task[currentPID]->counter == 0) {
            // if the remaining time is 0, increase zeroCount and NOT change shortestPID
            zeroCount++;
        } else if (task[currentPID]->counter < minCount) {
            // if remaining time > 0, judge if the shortestPID should be changed
            shortestPID = currentPID;
            minCount = task[currentPID]->counter;
        }
    }
    // if the all tasks' running time are 0, reset running time for all tasks
    if (zeroCount == NR_TASKS - 1) {
        for (currentPID = 1; currentPID < NR_TASKS; currentPID++) {
            task[currentPID]->counter = rand();
        }
        printk("\n");
        schedule(); // re-schedule
    }
    switch_to(task[shortestPID]);
    // back to do_timer()
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
    // the tail of switch_to is reached only when task is scheduled again(restored from __switch_to)
    // back to tail of schedule(), and all the way back to the start of _restore_context in _traps
}