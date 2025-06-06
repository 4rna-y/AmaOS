#include "LoaderBootInfo.h"

#include "memory/PhysicalPageAllocator.h"
#include "driver/Ahci.h"

#include "graphics/DirectFrameWriter.h"

#include <stddef.h>
#include <stdint.h>

extern "C" void k_main(LOADER_BOOT_INFO* bootInfo)
{
    uint8_t* frameBuffer = reinterpret_cast<uint8_t*>(bootInfo->frameBufferBase);
    uint64_t size = bootInfo->frameBufferSize;
    uint32_t width = bootInfo->frameBufferWidth;
    uint32_t height = bootInfo->frameBufferHeight;

    FRAME_BUFFER_INFO fbi = 
    { 
        .base = frameBuffer, 
        .size = size, 
        .pixelsPerScanLine = bootInfo->pixelsPerScanLine,
        .bytesPerPixel = bootInfo->bytesPerPixel,
        .isBGR = bootInfo->isBGR,
        .frameSize = (SIZE)
        { 
            .width = width, .height = height 
        } 
    };

    k_dfw_init(&fbi);
    k_dfw_clear();
    k_dfw_draw_string_f({0, 10}, 1.0f, {255, 255, 255}, "Hello world from Kernel!!");
    k_dfw_draw_string_f({0, 25}, 1.0f, {255, 255, 255}, "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,:;!?-=+=/\\()");
    k_dfw_draw_string_f({0, 40}, 2.0f, {20, 255, 20}, "scaleable and full color");
    k_dfw_draw_string_f({0, 60}, 1.0f, {255, 0, 0}, "int  format : %d", -123456);
    k_dfw_draw_string_f({0, 75}, 1.0f, {255, 0, 0}, "uint format : %u", 987);
    k_dfw_draw_string_f({0, 90}, 1.0f, {255, 0, 0}, "hex  format : 0x%x", 65535);

    for (;;) { __asm__ volatile("hlt"); }
}

