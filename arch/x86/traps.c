#include <stdint.h>
#include <stddef.h>

#include "include/ioport.h"
#include "include/printk.h"
#include "include/keyboard.h"
#include "include/signal.h"
#include "include/alarm.h"
#include "include/panic.h"

struct Idt
{
    uint16_t isr_low;
    uint16_t selector;
    uint8_t reserved;
    uint8_t type_attributes;
    uint16_t isr_high;

} __attribute__((packed));

struct IdtRegister
{
    uint16_t limit;
    long base;

} __attribute__((packed));

struct IdtRegister idtr;

struct Idt idt_table[256] __attribute__((aligned(8)));

void setIdtDescriptor(int vector, uint8_t type_attributes, void* isr)
{
    struct Idt* Idt = &idt_table[vector];
    Idt->isr_low = (long)isr & 0xFFFF;
    Idt->selector = 0x08;
    Idt->reserved = 0;
    Idt->type_attributes = type_attributes;
    Idt->isr_high = (long)isr >> 16 & 0xFFFF;
}

void divide_by_zero_error()
{
    printk("Divide by zero error");
}

void double_fault()
{
    panic("* KERNEL PANIC - Double Fault #DF *");
}

volatile long ticks = 0;

void clock_handler()
{
    outb(0x20, 0x20);
    ticks += 7;

    for (int i = 0; i < 64; ++i)
    {
        if (callouts[i].ticks != 0)
        {
            if (callouts[i].ticks <= ticks)
            {
                callouts[i].ticks = 0;
                sendsig(callouts[i].task, SIGALRM);
            }
        }
    }

    schedule();
}

void keyboard_handler()
{
    keyboard_handler_ext();
    outb(0x20, 0x20);
}

void TrapsInstall()
{
    setIdtDescriptor(0, 0x8E, divide_by_zero_error);
    setIdtDescriptor(0x08, 0x8E, double_fault);
    setIdtDescriptor(0x20, 0x8E, clock_handler);
    setIdtDescriptor(0x21, 0x8E, keyboard_handler);

    idtr.limit = sizeof(idt_table) - 1;
    idtr.base = (long)idt_table;

    __asm__ volatile ("lidt %0" :: "m"(idtr));
}
