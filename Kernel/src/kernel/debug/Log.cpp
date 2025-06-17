#include "debug/Log.h"

namespace debug
{
    void print(const char* str)
    {
        while(*str != '\0')
        {
            putc(*str++);
        }
    }

    void print(u64 value, bool hex)
    {    
        if (hex)
        {
            char buf[18];
            buf[17] = '\0';
            
            for (int i = 0; i < 16; ++i)
            {
                u8 digit = static_cast<u8>(value & 0xF);
                buf[16 - i] = (digit < 10) ? ('0' + digit) : ('A' + (digit - 10));
                value >>= 4;
            }
            
            buf[0] = '0';
            buf[1] = 'x';
            
            int idx = 2;
            while (idx < 16 && buf[idx] == '0')
            {
                ++idx;
            }
            if (idx == 16)
            {
                --idx;
            }
            for (; buf[idx] != '\0'; ++idx)
            {
                putc(buf[idx]);
            }
        }
        else
        {
            char buf[21];
            int  pos = 20;
            buf[pos] = '\0';

            if (value == 0)
            {
                buf[--pos] = '0';
            }
            else
            {
                while (value != 0 && pos > 0)
                {
                    u8 digit = static_cast<u8>(value % 10);
                    buf[--pos] = '0' + digit;
                    value /= 10;
                }
            }
            
            for (; buf[pos] != '\0'; ++pos)
            {
                putc(buf[pos]);
            }
        }
    }
}
