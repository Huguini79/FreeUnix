#ifndef IOPORT_H
#define IOPORT_H

#include <stdint.h>
#include <stddef.h>

uint8_t insb(unsigned short port);
unsigned short insw(unsigned short port);
uint32_t inl(unsigned short port);

void outb(unsigned short port, uint8_t data);
void outw(unsigned short port, unsigned short data);
void outl(unsigned short port, long data);

#endif