#include "include/signal.h"
#include "include/sched.h"
#include "include/string.h"

void sendsig(struct task_struct* task, long signal)
{
    __asm__ volatile ("cli");
    task->signal |= 1U << (signal);
    __asm__ volatile ("sti");
}

void psig()
{
    if (current->signal & (1U << SIGTERM))
    {
        printk("* SIGTERM for task with PID=");
        char buf[16];
        itoa(current->pid, buf, 10);
        printk(buf);
        printk(" *\n");
        current->signal &= ~(1U << SIGTERM);
    
    } else if (current->signal & (1U << SIGINT))
    {
        printk("* SIGINT for task with PID=");
        char buf[16];
        itoa(current->pid, buf, 10);
        printk(buf);
        printk(" *\n");
        current->signal &= ~(1U << SIGINT);
    
    } else if (current->signal & (1U << SIGALRM))
    {
        printk("* SIGALRM for task with PID=");
        char buf[16];
        itoa(current->pid, buf, 10);
        printk(buf);
        printk(" *\n");
        current->signal &= ~(1U << SIGALRM);
    }
}