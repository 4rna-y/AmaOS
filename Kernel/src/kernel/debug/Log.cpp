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
            char buf[20];
            buf[19] = '\0';
            buf[18] = '\n';
            
            u64 temp_val = value;
            for (int i = 0; i < 16; ++i)
            {
                u8 digit = static_cast<u8>(temp_val & 0xF);
                buf[17 - i] = (digit < 10) ? ('0' + digit) : ('A' + (digit - 10));
                temp_val >>= 4;
            }
            
            buf[0] = '0';
            buf[1] = 'x';

            putc(buf[0]);
            putc(buf[1]);

            int print_start_idx = 2;
            while (print_start_idx < 17 && buf[print_start_idx] == '0')
            {
                print_start_idx++;
            }

            for (int i = print_start_idx; i <= 17; ++i)
            {
                putc(buf[i]);
            }
            putc(buf[18]);
        }
        else
        {
            char buf[22];
            int current_idx = 21;

            buf[current_idx--] = '\0';
            buf[current_idx--] = '\n';

            u64 temp_val = value;
            if (temp_val == 0)
            {
                buf[current_idx--] = '0';
            }
            else
            {
                while (temp_val != 0 && current_idx >= 0)
                {
                    u8 digit = static_cast<u8>(temp_val % 10);
                    buf[current_idx--] = '0' + digit;
                    temp_val /= 10;
                }
            }

            for (int i = current_idx + 1; buf[i] != '\0'; ++i)
            {
                putc(buf[i]);
            }
        }
    }
}
