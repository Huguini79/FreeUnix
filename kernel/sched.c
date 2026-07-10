#include "include/sched.h"
#include "include/gdt.h"

#define CODE_SELECTOR 0x08
#define DATA_SELECTOR 0x10

typedef long pid_t;

struct task_struct tasks[64] = {0,};
struct task_struct* current = &tasks[0];
struct task_struct* next = &tasks[0];

void blank()
{
    while(1)__asm__ volatile ("sti");
}

struct task_struct* initProc(long pid, void* eip)
{
    struct task_struct* newTask = &tasks[pid];
    newTask->pid = pid;
    newTask->signal = 0;
    newTask->alarm = 0;
    newTask->state = Ready;

    newTask->tss.esp0 = 0x600000 + pid * 4096;
    newTask->tss.ss0 = 0x10;

    newTask->tss.eip = (long)eip;
    newTask->tss.eflags = 0x202;

    newTask->tss.eax = 0;
    newTask->tss.ecx = 0;
    newTask->tss.edx = 0;
    newTask->tss.ebx = 0;
    long* stack = (long*)0x3FF00 + pid * 8192;
    stack--;
    *stack = blank;
    newTask->tss.esp = stack;
    newTask->tss.ebp = 0;
    newTask->tss.esi = 0;
    newTask->tss.edi = 0;
    newTask->tss.cs = CODE_SELECTOR;
    newTask->tss.ds = DATA_SELECTOR;
    newTask->tss.ss = DATA_SELECTOR;
    newTask->tss.es = DATA_SELECTOR;
    newTask->tss.fs = DATA_SELECTOR;
    newTask->tss.gs = DATA_SELECTOR;

    newTask->tss.iopb = 0x80000000;

    setTSSDescriptor(newTask);

    return newTask;

}

void schedule()
{
    pid_t start = current->pid;
    pid_t next_pid = -1;

    for (int i = 1; i < 64; ++i)
    {
        pid_t pid = (start + i) % 64;

        if (tasks[pid].tss.eip != 0)
            next_pid = pid;

    }

    if (next_pid == -1)
        return;

    if (current->signal != 0)
    {
        psig();
    }

    current->state = Ready;
    current = &tasks[next_pid];
    current->state = Running;
    exec(current);
}

void exec(struct task_struct* task)
{
    unsigned short selector = (task->pid+6) * 8;

    volatile struct
    {
        long offset;
        unsigned short selector;

    } __attribute__((packed)) _tmp;
    
    _tmp.offset = 0;
    _tmp.selector = selector;

    __asm__ volatile ("ljmp %0" :: "m"(_tmp));

}