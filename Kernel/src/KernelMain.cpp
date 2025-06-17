#include "LoaderBootInfo.h"

#include "memory/PhysicalPageAllocator.h"
#include "memory/VirtualMemoryManager.h"
#include "memory/MemoryOperator.h"
#include "driver/Ahci.h"
#include "cpu/Cpu.h"
#include "panic/PanicFrameWriter.h"
#include "debug/Log.h"

#include "graphics/DirectFrameWriter.h"

#include <stddef.h>
#include <stdint.h>

static LOADER_BOOT_INFO* g_boot_info = nullptr;

KERNEL_STATUS init();

static inline void halt()
{
    for (;;) { __asm__ volatile("hlt"); }
}

static inline void dbg_putc(char c)
{
    __asm__ volatile("outb %0, $0xE9" :: "a"(c));
}

extern "C" void k_main(LOADER_BOOT_INFO* bootInfo)
{
    g_boot_info = bootInfo;
    if (init() == KERNEL_FAILURE) return;

    volatile uint64_t* bad = reinterpret_cast<uint64_t*>(0x100000000);
    *bad = 0xDEADBEEF; // PF

    halt();
}

KERNEL_STATUS init()
{
    stdcharbmp::init();
    panic::init(g_boot_info);
    cpu::init_bsp();

    KERNEL_STATUS dfwStatus = dfw::init(g_boot_info);
    if (dfwStatus == KERNEL_FAILURE) return KERNEL_FAILURE;
    
    dfw::clear();
    uint64_t yOffsetMagn = 1;

    dfw::draw_string_f({0, 14 * yOffsetMagn++}, { 1.0f, 1.2f }, { 255, 255, 255 }, "inited dfw.");

    dfw::draw_string_f({0, 14 * yOffsetMagn++}, { 1.0f, 1.2f }, { 255, 255, 255 }, "%x", g_boot_info->frameBufferBase);
    
    KERNEL_STATUS ppaStatus = ppa::init(g_boot_info);
    if (ppaStatus == KERNEL_FAILURE)
    {
        dfw::draw_string_f({0, 14 * yOffsetMagn++}, { 1.0f, 1.2f }, { 255, 255, 255 }, "failed to init ppa");
        return KERNEL_FAILURE;
    }
    dfw::draw_string_f({0, 14 * yOffsetMagn++}, { 1.0f, 1.2f }, { 255, 255, 255 }, "inited ppa.");

    KERNEL_STATUS vmmStatus = vmm::init(g_boot_info);
    if (vmmStatus == KERNEL_FAILURE)
    {
        dfw::draw_string_f({0, 14 * yOffsetMagn++}, { 1.0f, 1.2f }, { 255, 255, 255 }, "failed to init vmm");
        return KERNEL_FAILURE;
    }

    dfw::switchHigherHalf();
    dfw::draw_string_f({0, 14 * yOffsetMagn++}, { 1.0f, 1.2f }, { 255, 255, 255 }, "%x", g_boot_info->frameBufferBase);

    dfw::draw_string_f({0, 14 * yOffsetMagn++}, { 1.0f, 1.2f }, { 255, 255, 255 }, "inited gdt.");

    // auto outb = [](u16 port, u8 val)
    // { 
    //     __asm__ volatile ("outb %0, %1" :: "a"(val), "Nd"(port)); 
    // };
    // outb(0x21, 0xFF);
    // outb(0xA1, 0xFF);
    __asm__ volatile("sti");

    dfw::draw_string_f({0, 14 * yOffsetMagn++}, { 1.0f, 1.2f }, { 255, 255, 255 }, "inited vmm.");
    
    //KERNEL_STATUS ahciStatus = ahci::init();

    return KERNEL_SUCCESS;
}

