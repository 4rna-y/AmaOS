#pragma once

#include "Types.h"
#include "KernelStatus.h"
#include "LoaderBootInfo.h"

namespace vmm
{
    static constexpr u64 PAGE_SIZE = 4096ULL;
    static constexpr u64 PAGE_OFFSET_MASK = PAGE_SIZE - 1;
    static constexpr u64 PAGE_ADDR_MASK = ~PAGE_OFFSET_MASK;
    
    static constexpr u64 HIGHER_HALF_OFFSET = 0xFFFF800000000000ULL;

    static constexpr u64 LAPIC_BASE = 0xFEE00000ULL;
    static constexpr u64 IOAPIC_BASE = 0xFEC00000ULL;
    static constexpr u64 MMIO_SIZE   = 0x1000ULL;
    static constexpr u64 PCI_MMIO_BASE = 0xC0000000ULL;
    static constexpr u64 PCI_MMIO_SIZE = 0x40000000ULL;
    static constexpr u64 MAP_HUGE = 1ULL;
    static constexpr u64 PHYS_MASK_2MIB = ~0x1FFFFFULL;

    enum PageFlags : u64
    {
        PF_P = 1ULL << 0,
        PF_RW = 1ULL << 1,
        PF_US = 1ULL << 2,
        PF_PWT = 1ULL << 3,
        PF_PCD = 1ULL << 4,
        PF_PS = 1ULL << 7,
        PF_NX = 1ULL << 63
    };

    KERNEL_STATUS init(LOADER_BOOT_INFO* info);
    void map_page(u64 virt, u64 phys, u64 flags);
    void unmap_page(u64 virt);
    void map_range(u64 virt, u64 physStart, u64 size, u64 flags, u64 opts = 0);
    void unmap_range(u64 virt, u64 size);
    u64 get_phys(u64 virt);
    inline u64 virt2phys(u64 virt)
    {
        if (virt >= HIGHER_HALF_OFFSET) return virt - HIGHER_HALF_OFFSET;
        return virt;
    }
}