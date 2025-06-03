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

    k_dfw_draw_box(&fbi, { 0, 0 }, { width, height }, { 255, 0, 0 });
    k_dfw_draw_box(&fbi, { 10, 10 }, { 10, 10 }, { 0, 255, 0 });

    for (;;) { __asm__ volatile("hlt"); }
    
}

