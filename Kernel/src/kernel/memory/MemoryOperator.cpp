#include "memory/MemoryOperator.h"

namespace memop
{
    void* set_mem(void* dest, size_t len, int value)
    {
        u8 *destOcted = (u8*)dest;
        size_t qwCount = len / 8;
        size_t byteRemaining = len % 8;
        
        u64 val = (u8)value;
        val |= val << 8;
        val |= val << 16;
        val |= val << 32;

        __asm__ volatile(
            "rep stosq" 
            : "+D"(destOcted), "+c"(qwCount) 
            : "a"(val)
            : "memory"
        );

        __asm__ volatile(
            "rep stosb" 
            : "+D"(destOcted), "+c"(byteRemaining) 
            : "a"(val) 
            : "memory"
        );

        return dest;
    }

    void* zero_mem(void* dest, size_t len)
    {
        u8 *destOcted = (u8*)dest;
        size_t qwCount = len / 8;
        size_t byteRemaining = len % 8;
        
        u64 val = (u8)0;
        val |= val << 8;
        val |= val << 16;
        val |= val << 32;

        __asm__ volatile(
            "rep stosq" 
            : "+D"(destOcted), "+c"(qwCount) 
            : "a"(val) 
            : "memory"
        );

        __asm__ volatile(
            "rep stosb" 
            : "+D"(destOcted), "+c"(byteRemaining) 
            : "a"(val) 
            : "memory"
        );

        return dest;
    }
}