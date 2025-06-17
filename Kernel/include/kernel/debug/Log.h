#pragma once

#include "Types.h"

namespace debug
{
    static inline void putc(char c)
    {
        __asm__ volatile("outb %0, $0xE9" :: "a"(c));
    }

    void print(const char* str);
    void print(u64 value, bool hex = false);
}