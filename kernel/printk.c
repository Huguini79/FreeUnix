#include "include/console.h"
#include "include/string.h"

#include <stdint.h>
#include <stddef.h>

void printk(const char* str)
{
    size_t len = strlen(str);

    for (int i = 0; i < len; ++i)
    {
        put_c(str[i]);
    }
}