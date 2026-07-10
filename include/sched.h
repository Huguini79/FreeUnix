#ifndef SCHED_H
#define SCHED_H

#define MAX_TASKS 64

/*
    --------------------------
    TSS - Task State Structure
    --------------------------
*/
struct tss
{
    long prev_link;
    long esp0;
    long ss0;
    long esp1;
    long ss1;
    long esp2;
    long ss2;
    long cr3;
    long eip;
    long eflags;
    long eax;
    long ecx;
    long edx;
    long ebx;
    long esp;
    long ebp;
    long esi;
    long edi;
    long es;
    long cs;
    long ss;
    long ds;
    long fs;
    long gs;
    long ldtr;
    long iopb;

} __attribute__((packed));

/*
    ---------------------------
    PCB - Process Control Block
    ---------------------------
*/

enum State
{
    Ready,
    Running,
    Zombie,
    Sleep,
};

struct task_struct
{
    long pid;
    long ppid;
    long signal;
    long alarm;
    enum State state;
    struct tss tss;
};

extern struct task_struct tasks[MAX_TASKS];
extern struct task_struct* current;

struct task_struct* initProc(long pid, void* eip);
void schedule();
void exec(struct task_struct* task);

#endif