#include "include/kernel.h"
#include "include/console.h"
#include "include/printk.h"
#include "include/gdt.h"
#include "include/alarm.h"
#include "include/traps.h"
#include "include/pic.h"
#include "include/sched.h"

void proc1()
{printk("Proc1\n");alarm(3);for(;;)__asm__ volatile ("sti");}

void proc2()
{printk("Proc2\n");for(;;)__asm__ volatile ("sti");}

void proc3()
{printk("Proc3\n");for(;;)__asm__ volatile ("sti");}

void proc4()
{printk("Proc4\n");for(;;)__asm__ volatile ("sti");}

void proc5()
{printk("Proc5\n");for(;;)__asm__ volatile ("sti");}

void kernel_init()
{
    remapPIC();
    GdtInstall();
    TrapsInstall();
    __asm__ volatile ("sti");
    ConsoleInstall();
    printk("FreeUnix 1.0 version\n\n");
    
    struct task_struct* process1 = initProc(1, proc1);
    struct task_struct* process2 = initProc(2, proc2);
    struct task_struct* process3 = initProc(3, proc3);
    struct task_struct* process4 = initProc(4, proc4);
    struct task_struct* process5 = initProc(5, proc5);

    while(1)
    {
        __asm__ volatile ("sti");
    }
}
