#include "graphics/DirectFrameWriter.h"

#include <stdarg.h>

#include "memory/VirtualMemoryManager.h"

#include "Math.h"

namespace dfw
{
    static FRAME_BUFFER_INFO g_fb_info = { 0 };
    static bool g_init = false;

    KERNEL_STATUS init(LOADER_BOOT_INFO* info)
    {
        if (g_init) return KERNEL_FAILURE;

        g_init = true;

        g_fb_info.base = reinterpret_cast<u8*>(info->frameBufferBase), 
        g_fb_info.size = info->frameBufferSize, 
        g_fb_info.pixelsPerScanLine = info->pixelsPerScanLine,
        g_fb_info.bytesPerPixel = info->bytesPerPixel,
        g_fb_info.isBGR = info->isBGR,
        g_fb_info.frameSize = (SIZE)
        { 
            .width = info->frameBufferWidth, 
            .height = info->frameBufferHeight 
        };

        return KERNEL_SUCCESS;
    }

    KERNEL_STATUS switchHigherHalf()
    {
        g_fb_info.base = reinterpret_cast<u8*>(g_fb_info.base + vmm::HIGHER_HALF_OFFSET);
        return KERNEL_SUCCESS;
    }

    KERNEL_STATUS clear()
    {
        if (!g_init) return KERNEL_FAILURE;
        for (u64 i = 0; i < g_fb_info.size; i++)
        {
            g_fb_info.base[i] = 0;
        }

        return KERNEL_SUCCESS;
    }

    KERNEL_STATUS draw_pixel(POINT point, COLOR color)
    {
        if (!g_init) return KERNEL_FAILURE;

        if (point.x >= g_fb_info.frameSize.width || point.y >= g_fb_info.frameSize.height) 
            return KERNEL_FAILURE;

        u64 bufferIdx = (u64)(point.x + point.y * g_fb_info.pixelsPerScanLine) * g_fb_info.bytesPerPixel;

        if (g_fb_info.isBGR) 
        {
            g_fb_info.base[bufferIdx + 0] = color.blue;
            g_fb_info.base[bufferIdx + 1] = color.green;
            g_fb_info.base[bufferIdx + 2] = color.red;
            g_fb_info.base[bufferIdx + 3] = 0;
        } 
        else 
        {
            g_fb_info.base[bufferIdx + 0] = color.red;
            g_fb_info.base[bufferIdx + 1] = color.green;
            g_fb_info.base[bufferIdx + 2] = color.blue;
            g_fb_info.base[bufferIdx + 3] = 0;
        }

        return KERNEL_SUCCESS;
    }

    KERNEL_STATUS draw_box(POINT point, SIZE size, COLOR color)
    {
        if (!g_init) return KERNEL_FAILURE;

        if (point.x >= g_fb_info.frameSize.width || point.y >= g_fb_info.frameSize.height) 
            return KERNEL_FAILURE;

        if (size.width == 0 || size.height == 0) return KERNEL_FAILURE;

        if (point.x + size.width  > g_fb_info.frameSize.width || point.y + size.height > g_fb_info.frameSize.height) 
            return KERNEL_FAILURE;

        u32 pitchPixels = g_fb_info.pixelsPerScanLine;
        u32 bpp = g_fb_info.bytesPerPixel;
        u64 fbBytes = g_fb_info.size;

        for (u32 y = 0; y < size.height; ++y) 
        {
            for (u32 x = 0; x < size.width; ++x) 
            {
                u32 dx = point.x + x;
                u32 dy = point.y + y;

                u64 pixelIndex = (u64)dy * pitchPixels + dx;
                u64 bufferIdx  = pixelIndex * bpp;

                if (bufferIdx + bpp - 1 >= fbBytes) continue;
                
                if (g_fb_info.isBGR) 
                {
                    g_fb_info.base[bufferIdx + 0] = color.blue;
                    g_fb_info.base[bufferIdx + 1] = color.green;
                    g_fb_info.base[bufferIdx + 2] = color.red;
                    g_fb_info.base[bufferIdx + 3] = 0;
                } 
                else 
                {
                    g_fb_info.base[bufferIdx + 0] = color.red;
                    g_fb_info.base[bufferIdx + 1] = color.green;
                    g_fb_info.base[bufferIdx + 2] = color.blue;
                    g_fb_info.base[bufferIdx + 3] = 0;
                }
            }
        }

        return KERNEL_SUCCESS;
    }

    static char* itoa(char* buffer, int bufferSize, int value)
    {
        if (bufferSize < 2) return buffer;

        bool neg = false;
        unsigned int v;

        if (value < 0)
        {
            neg = true;
            v = (unsigned int)(-value);
        }
        else
        {
            v = (unsigned int)value;
        }

        int i = bufferSize - 1;
        buffer[i--] = '\0';

        if (v == 0)
        {
            buffer[i--] = '0';
        }
        else
        {
            while (v > 0 && i >= 0)
            {
                buffer[i--] = (char)('0' + (v % 10));
                v /= 10;
            }
        }

        if (neg && i >= 0)
        {
            buffer[i--] = '-';
        }

        return &buffer[i + 1];
    }

    static char* utoa(char* buffer, int bufferSize, unsigned int value)
    {
        if (bufferSize < 2) return buffer;

        int i = bufferSize - 1;
        buffer[i--] = '\0';

        if (value == 0)
        {
            buffer[i--] = '0';
        }
        else
        {
            while (value > 0 && i >= 0)
            {
                buffer[i--] = (char)('0' + (value % 10));
                value /= 10;
            }
        }

        return &buffer[i + 1];
    }

    static char* uhextoa(char* buffer, int bufferSize, unsigned int value)
    {
        if (bufferSize < 2) return buffer;

        int i = bufferSize - 1;
        buffer[i--] = '\0';

        if (value == 0)
        {
            buffer[i--] = '0';
        }
        else
        {
            while (value > 0 && i >= 0)
            {
                unsigned int digit = value & 0xF;
                buffer[i--] = (char)(digit < 10 ? ('0' + digit) : ('A' + (digit - 10)));
                value >>= 4;
            }
        }

        return &buffer[i + 1];
    } 

    static int get_vsf(char* buffer, int bufferSize, const char* fmt, va_list ap)
    {
        int pos = 0;

        for (const char* p = fmt; *p != '\0'; p++)
        {
            if (*p != '%')
            {
                if (pos + 1 < bufferSize) buffer[pos++] = *p;
                continue;
            }

            p++;
            if (*p == '\0') break;

            switch (*p)
            {
                case '%': 
                {
                    if (pos + 1 < bufferSize) buffer[pos++] = '%';
                    break;
                }
                case 'c':
                {
                    char c = (char)va_arg(ap, int);
                    if (pos + 1 < bufferSize) buffer[pos++] = c;
                    break;
                }
                case 's':
                {
                    const char* s = va_arg(ap, const char*);
                    if (s == nullptr) s = "[null]";
                    while (*s != '\0' && pos + 1 < bufferSize) buffer[pos++] = *s++;
                    break;
                }
                case 'd':
                {
                    int v = va_arg(ap, int);
                    char tmp[16];
                    char* s = itoa(tmp, sizeof(tmp), v);
                    while (*s != '\0' && pos + 1 < bufferSize) buffer[pos++] = *s++;
                    break;
                }
                case 'u':
                {
                    unsigned int v = va_arg(ap, unsigned int);
                    char tmp[16];
                    char* s = utoa(tmp, sizeof(tmp), v);
                    while (*s != '\0' && pos + 1 < bufferSize) buffer[pos++] = *s++;
                    break;
                }
                case 'x':
                {
                    unsigned int v = va_arg(ap, unsigned int);
                    char tmp[16];
                    char* s = uhextoa(tmp, sizeof(tmp), v);
                    while (*s != '\0' && pos + 1 < bufferSize) buffer[pos++] = *s++;
                    break;
                }
                default:
                {
                    if (pos + 1 < bufferSize) buffer[pos++] = *p;
                    break;
                }
            }  
        }

        if (pos < bufferSize) buffer[pos] = '\0';
        else buffer[bufferSize - 1] = '\0';
        
        return pos;
    }

    KERNEL_STATUS draw_char(POINT point, SIZE_F scale, COLOR color, char c)
    {
        if (!g_init) return KERNEL_FAILURE;

        const uint8_t* glyph = stdcharbmp::get_char_bitmap(c);
        if (glyph == nullptr) return KERNEL_FAILURE;
    
        int outWidth = (int)floor_f(CHARACTOR_BITMAP_HEIGHT * scale.width);
        int outHeight = (int)floor_f(CHARACTOR_BITMAP_HEIGHT * scale.height);
        if (outWidth <= 0 || outHeight <= 0) return KERNEL_FAILURE;

        for (int dy = 0; dy < outHeight; dy++)
        {
            float sy = dy / scale.height;
            int iy = (int)floor_f(sy);
            
            if (iy < 0) iy = 0;
            else if (iy >= CHARACTOR_BITMAP_HEIGHT) iy = CHARACTOR_BITMAP_HEIGHT - 1;

            uint8_t rowBits = glyph[iy];
            for (int dx = 0; dx < outWidth; dx++)
            {
                float sx = dx / scale.width;
                int ix = (int)floor_f(sx);

                if (ix < 0) ix = 0;
                else if (ix >= CHARACTOR_BITMAP_HEIGHT) ix = CHARACTOR_BITMAP_HEIGHT - 1;

                bool flag = (rowBits & (0x80 >> ix)) != 0;
                if (flag) draw_pixel({ point.x + dx, point.y + dy }, color);
            }
        }

        return KERNEL_SUCCESS;
    }

    KERNEL_STATUS draw_string_f(POINT point, SIZE_F scale, COLOR color, const char* fmt, ...)
    {
        if (!g_init) return KERNEL_FAILURE;

        char buffer[256];
        va_list ap;
        va_start(ap, fmt);
        get_vsf(buffer, sizeof(buffer), fmt, ap);
        va_end(ap);

        uint64_t x = point.x;
        uint64_t y = point.y;
        int xStride = (int)floor_f(CHARACTOR_BITMAP_HEIGHT * scale.width);
        int yStride = (int)floor_f(CHARACTOR_BITMAP_HEIGHT * scale.height);
        if (xStride <= 0) xStride = 1;
        if (yStride <= 0) yStride = 1;

        for (const char* p = buffer; *p != '\0'; p++)
        {
            draw_char({ x, y }, scale, color, *p);
            x += xStride;
            if (*p == '\n') {
                x = point.x;
                y += yStride;
            }
        }

        return KERNEL_SUCCESS;
    }
}