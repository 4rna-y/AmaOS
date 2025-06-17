#include "panic/PanicFrameWriter.h"
#include "panic/PanicInfo.h"
#include "graphics/StandardCharactorBitmapProvider.h"

constexpr u32 FONT_WIDTH = 8;
constexpr u32 FONT_HEIGHT = 8;

inline constexpr u32 create_rgb(u8 red, u8 green, u8 blue)
{
    return 
        (static_cast<u32>(red) << 16) | 
        (static_cast<u32>(green) << 8) | 
        static_cast<u32>(blue);
}

static const LOADER_BOOT_INFO* g_boot = nullptr;

void to_u64hex(u64 v, char out[19]) noexcept
{
    static const char hex[] = "0123456789ABCDEF";
    out[0] = '0';
    out[1] = 'x';
    for (int i = 0; i < 16; ++i)
    {
        out[2 + i] = hex[(v >> (60 - (i * 4))) & 0xF];
    }
    out[18] = '\0';
}

inline void draw_pixel(u32 x, u32 y, u32 color) noexcept
{
    u32* frameBase = reinterpret_cast<u32*>(g_boot->frameBufferBase);
    u32 pitch = (g_boot->pixelsPerScanLine * g_boot->bytesPerPixel) / sizeof(u32);
    frameBase[y * pitch + x] = color; 
}

void draw_char(u32 x, u32 y, char c, u32 color) noexcept
{
    const u8* bmp = stdcharbmp::get_char_bitmap(c);
    for (u32 dy = 0; dy < FONT_HEIGHT; ++dy)
    {
        u8 bits = bmp[dy];
        for (u32 dx = 0; dx < FONT_WIDTH; ++dx)
        {
            if (bits & (1 << (7 - dx)))
            {
                draw_pixel(x + dx, y + dy, color);
            }
        }
    }
}

namespace panic
{
    void init(const LOADER_BOOT_INFO* info) noexcept
    {
        g_boot = info;
    }

    __attribute__((no_caller_saved_registers))
    void clear(u8 red, u8 green, u8 blue) noexcept
    {
        if (!g_boot) return;

        u8* f = reinterpret_cast<u8*>(g_boot->frameBufferBase);
        for (u64 i = 0; i < g_boot->frameBufferSize; i++)
        {
            f[i] = 255;
        }
    }

    __attribute__((no_caller_saved_registers))
    void draw_string(u32 x, u32 y, const char* str) noexcept
    {
        if (!g_boot) return;

        const u32 white = create_rgb(255, 255, 255);
        for (u32 i = 0; str[i] != '\0'; ++i)
        {
            draw_char(x + (i * FONT_WIDTH), y, str[i], white);
        }
    }

    __attribute__((no_caller_saved_registers))
    void panic(const PANIC_INFO& info) noexcept
    {
        __asm__ volatile("cli");
        clear(0, 0, 128);

        u32 y = 0;
        draw_string(10, FONT_HEIGHT * y++, "*** KERNEL PANIC ***");

        char buf[19];
        static const char* regs[16] = 
        {
            "RAX", "RBX", "RCX", "RDX",
            "RSI", "RDI", "RBP", "RSP",
            " R8", " R9", "R10", "R11",
            "R12", "R13", "R14", "R15"
        };

        draw_string(10, y, "Fault Addr: ");
        to_u64hex(info.faultAddr, buf);
        draw_string(11 * FONT_WIDTH, FONT_HEIGHT * y++, buf);
        
        draw_string(10, y, "Error Code: ");
        to_u64hex(info.errorCode, buf);
        draw_string(11 * FONT_WIDTH, FONT_HEIGHT * y++, buf);

        draw_string(10, y, "RIP: ");
        to_u64hex(info.rip, buf);
        draw_string(11 * FONT_WIDTH, FONT_HEIGHT * y++, buf);

        for (int i = 0; i < 16; ++i)
        {
            draw_string(10, y, regs[i]);
            draw_string(4 * FONT_WIDTH, y, ": ");
            to_u64hex(info.registers[i], buf);
            draw_string(11 * FONT_WIDTH, FONT_HEIGHT * y++, buf);
        }

        for (;;) __asm__ volatile("hlt");
    }
}