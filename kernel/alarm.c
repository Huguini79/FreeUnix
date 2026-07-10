#include "include/alarm.h"

struct callout callouts[64] = {0};

void alarm(long s)
{
    callouts[current->pid].ticks = (s * 1000) / 10;
    callouts[current->pid].task = current;
}