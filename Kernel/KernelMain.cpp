#include "BootInfo.h"
#include <stddef.h>
#include <stdint.h>

extern "C" void KernelMain(BootInfo* bootInfo)
{
    uint8_t* frameBuffer = reinterpret_cast<uint8_t*>(bootInfo->frameBufferBase);
    uint64_t size = bootInfo->frameBufferSize;

    for (uint64_t i = 0; i < size; ++i)
    {
        frameBuffer[i] = 255;
    }

    for (;;)
    {
        __asm__ volatile("hlt");
    }
}


    