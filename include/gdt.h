#ifndef GDT_H
#define GDT_H

#include "include/sched.h"

void GdtInstall();
void setTSSDescriptor(struct task_struct* task);

#endif
