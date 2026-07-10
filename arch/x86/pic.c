#include "include/ioport.h"

#include <stdint.h>
#include <stddef.h>

void remapPIC()
{
    /* ICW 1 - Initialize Slave and Master PIC */
    unsigned char packet_icw1 = 0x11;
    outb(0x20, packet_icw1);
    outb(0xA0, packet_icw1);

    /* ICW 2 - Map the IRQs */
    unsigned char packet1_icw2 = 0x20;
    unsigned char packet2_icw2 = 0x28;
    outb(0x21, packet1_icw2);
    outb(0xA1, packet2_icw2);

    /* ICW 3 - Select Slave PIC -> IR Line 2 */
    unsigned char packet1_icw3 = 0x4;
    unsigned char packet2_icw3 = 0x2;
    outb(0x21, packet1_icw3);
    outb(0xA1, packet2_icw3);

    /* ICW 4 - Select 80x86 mode */
    unsigned char packet_icw4 = 1;
    outb(0x21, packet_icw4);
    outb(0xA1, packet_icw4);
}