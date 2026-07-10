#include <stdint.h>
#include <stddef.h>

#include "include/console.h"
#include "include/ioport.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define DEFAULT_FONT 0x0F

unsigned short* video_mem;

int x, y = 0;

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (insb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (insb(0x3D5) & 0xE0) | cursor_end);
}

void update_cursor(int x, int y)
{
    unsigned short pos = ((y + 1) * VGA_WIDTH) + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char) ((pos >> 8) & 0xFF));
}

void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void ConsoleInstall()
{
    video_mem = (unsigned short*)0xB8000;
    clear();
}

void del()
{
    if (x > 2)
    {
        x--;
        update_cursor(x, y);
        video_mem[(y * VGA_WIDTH) + x] = 0x0F << 8 | ' ';
    }
}

// void scroll()
// {
//     for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
//     {
//         video_mem[i] = video_mem[i + VGA_WIDTH];
//     }
// }

void put_c(char c)
{

    if (x >= VGA_WIDTH)
    {
        x = 0;
        y++;
        update_cursor(x, y);
    }

    if (y >= VGA_HEIGHT)
    {
        clear();
        update_cursor(x, y);
    }

    if (c == '\n')
    {
        x = 0;
        y++;
        update_cursor(x, y);
    
    } else
    {
        video_mem[(y * VGA_WIDTH) + x] = DEFAULT_FONT << 8 | c;
        x++;
        update_cursor(x, y);
    }
}

void clear()
{
    disable_cursor();
    x = 0;
    y = 0;
    for (int i = 0; i < VGA_HEIGHT; ++i)
    {
        for (int j = 0; j < VGA_WIDTH; ++j)
        {
            put_c(' ');
        }
    }

    x = 0;
    y = 0;

    enable_cursor(0, 1);
}
