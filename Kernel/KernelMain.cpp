#include "LoaderBootInfo.h"

#include "memory/PhysicalPageAllocator.h"

#include <stddef.h>
#include <stdint.h>

extern "C" void k_main(LOADER_BOOT_INFO* bootInfo)
{
    uint8_t c = 255;
    if (k_ppa_init(bootInfo) != KERNEL_SUCCESS)
    {
        c = 0;
    }

    void* p = k_ppa_alloc();
    if (p == nullptr)
    {
        c = 0;
    }

    if (k_ppa_free(p) != KERNEL_SUCCESS)
    {
        c = 128;
    }

    uint8_t* frameBuffer = reinterpret_cast<uint8_t*>(bootInfo->frameBufferBase);
    uint64_t size = bootInfo->frameBufferSize;

    for (uint64_t i = 0; i < size; ++i)
    {
        frameBuffer[i] = c;
    }

    for (;;)
    {
        __asm__ volatile("hlt");
    }
}


    