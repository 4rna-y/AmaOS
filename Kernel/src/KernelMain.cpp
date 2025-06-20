#include "LoaderBootInfo.h"

#include "memory/PhysicalPageAllocator.h"
#include "memory/VirtualMemoryManager.h"
#include "memory/MemoryOperator.h"
#include "driver/Ahci.h"
#include "cpu/Cpu.h"
#include "cpu/Exception.h"
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

    debug::print("assa");

    halt();
}

KERNEL_STATUS init()
{
    stdcharbmp::init();
    panic::init(g_boot_info);
    cpu::init_bsp();

    debug::print("[KernelMain::init] cpu initialized.\n");

    KERNEL_STATUS dfwStatus = dfw::init(g_boot_info);
    if (dfwStatus == KERNEL_FAILURE) 
    {
        debug::print("[KernelMain::init] Failed to init dfw\n");
        return KERNEL_FAILURE;
    }    
    
    debug::print("[KernelMain::init] dfw initialized.\n");

    dfw::clear();
    
    KERNEL_STATUS ppaStatus = ppa::init(g_boot_info);
    if (ppaStatus == KERNEL_FAILURE)
    {
        debug::print("[KernelMain::init] Failed to init ppa\n");
        return KERNEL_FAILURE;
    }

    debug::print("[KernelMain::init] ppa initialized.\n");

    KERNEL_STATUS vmmStatus = vmm::init(g_boot_info);
    if (vmmStatus == KERNEL_FAILURE)
    {
        debug::print("[KernelMain::init] Failed to init vmm\n");
        return KERNEL_FAILURE;
    }

    debug::print("[KernelMain::init] vmm initialized.\n");

    dfw::switchHigherHalf();
    
    //KERNEL_STATUS ahciStatus = ahci::init();

    return KERNEL_SUCCESS;
}

