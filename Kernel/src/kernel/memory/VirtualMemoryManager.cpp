#include "memory/VirtualMemoryManager.h"

#include <stddef.h>

#include "memory/PhysicalPageAllocator.h"
#include "memory/MemoryOperator.h"

namespace vmm
{
    static inline u64 rdmsr(u32 msr)
    {
        u32 lo, hi;
        __asm__ volatile
        (
            "rdmsr" 
            : "=a"(lo), "=d"(hi) 
            : "c"(msr)
        );
        return ((u64)hi << 32) | lo;
    }   

    static inline void wrmsr(u32 msr, u64 v)
    {
        u32 lo = (u32)v;
        u32 hi = (u32)(v >> 32);
        __asm__ volatile
        (
            "wrmsr" 
            :
            : "c"(msr), "a"(lo), "d"(hi)
        );
    }

    static inline size_t PML4_IDX(u64 v)
    {
        return (v >> 39) & 0x1FF;
    }

    static inline size_t PDPT_IDX(u64 v) 
    {
        return (v >> 30) & 0x1FF;
    }

    static inline size_t PD_IDX(u64 v)
    {
        return (v >> 21) & 0x1FF;
    }

    static inline size_t PT_IDX(u64 v)
    {
        return (v >> 12) & 0x1FF;
    }

    bool is_mappable(u32 t)
    {
        return 
            t == LoaderConventionalMemory ||
            t == LoaderBootServicesCode || 
            t == LoaderBootServicesData ||
            t == LoaderLoaderCode ||
            t == LoaderLoaderData ||
            t == LoaderACPIReclaimMemory ||
            t == LoaderACPIMemoryNVS ||
            t == LoaderMemoryMappedIO ||
            t == LoaderMemoryMappedIOPortSpace;
    }

    static u64* g_pml4_v = nullptr;
    static u64 g_pml4_p = 0;
    static bool g_hh_enabled = false;

    static inline u64* phys2virt(u64 phys)
    {
        if (phys == 0) return nullptr;
        u64 offset = g_hh_enabled ? HIGHER_HALF_OFFSET : 0;
        return reinterpret_cast<u64*>(phys + offset);
    }

    static u64 alloc_phys_page_zeroed()
    {
        void* p = ppa::alloc();
        if (p == nullptr) return 0;
        
        memop::zero_mem(p, PAGE_SIZE);
        return reinterpret_cast<uint64_t>(p);
    }

    static u64* ensure_table(u64* table, size_t idx)
    {
        if (!(table[idx] & PF_P))
        {
            u64 newPage = alloc_phys_page_zeroed();
            if (newPage == 0) return nullptr;

            table[idx] = newPage | PF_P | PF_RW;
        }

        u64 childTable = table[idx] & PAGE_ADDR_MASK;
        return phys2virt(childTable);
    }

    KERNEL_STATUS init(LOADER_BOOT_INFO* info)
    {
        g_pml4_p = alloc_phys_page_zeroed();
        if (g_pml4_p == 0) return KERNEL_FAILURE;

        g_pml4_v = phys2virt(g_pml4_p);
        if (g_pml4_v == nullptr) return KERNEL_FAILURE;

        g_pml4_v[510] = g_pml4_p | PF_P | PF_RW;

        u64 mmioFlags = PF_P | PF_RW | PF_PCD | PF_PWT;
        map_range(LAPIC_BASE, LAPIC_BASE, MMIO_SIZE, mmioFlags);
        map_range(IOAPIC_BASE, IOAPIC_BASE, MMIO_SIZE, mmioFlags);

        u64 pciMmioFlags = PF_P | PF_RW | PF_PCD | PF_PWT | PF_NX;
        map_range(PCI_MMIO_BASE, PCI_MMIO_BASE, PCI_MMIO_SIZE, pciMmioFlags);
        map_range(HIGHER_HALF_OFFSET + PCI_MMIO_BASE, PCI_MMIO_BASE, PCI_MMIO_SIZE, pciMmioFlags);

        u8* desc = (u8*)info->memoryMap;
        for (u64 off = 0; off < info->memoryMapSize; off += info->mapDescriptorSize)
        {
            LOADER_MEMORY_DESCRIPTOR* d = (LOADER_MEMORY_DESCRIPTOR*)(desc + off);
            u64 start = d->physicalStart;
            u64 size = d->numberOfPages * PAGE_SIZE;
            u64 opt = 0; 

            if (d->type == LoaderConventionalMemory || 
                d->type == LoaderBootServicesCode || 
                d->type == LoaderBootServicesData)
                opt = MAP_HUGE;

            map_range(start, start, size, PF_P | PF_RW, opt);
            if (is_mappable(d->type))
            {
                map_range(HIGHER_HALF_OFFSET + start, start, size, PF_P | PF_RW, opt);
            }
        }

        map_range(info->frameBufferBase, info->frameBufferBase, info->frameBufferSize, PF_P | PF_RW | PF_NX, MAP_HUGE);
        map_range(HIGHER_HALF_OFFSET + info->kernelBase, info->kernelBase, info->kernelSize, PF_P | PF_RW, MAP_HUGE);
        map_range(HIGHER_HALF_OFFSET + info->frameBufferBase, info->frameBufferBase, info->frameBufferSize, PF_P | PF_RW | PF_NX, MAP_HUGE);
        
        __asm__ volatile
        (
            "mov %0, %%cr3" 
            :
            : "r"(g_pml4_p) 
            : "memory"
        );

        u64 cr4;
        __asm__ volatile
        (
            "mov %%cr4, %0" 
            : "=r"(cr4)
        );
        cr4 |= (1ULL << 5);
        cr4 |= (1ULL << 7);
        __asm__ volatile
        (
            "mov %0, %%cr4" 
            :
            : "r"(cr4)
        );

        u64 efer = rdmsr(0xC0000080);
        efer |= (1ULL << 8);
        efer |= (1ULL << 11);
        wrmsr(0xC0000080, efer);

        u64 cr0;
        __asm__ volatile
        (
            "mov %%cr0, %0" 
            : "=r"(cr0)
        );
        cr0 |= (1ULL << 31);
        cr0 |= (1ULL << 16);
        __asm__ volatile
        (
            "mov %0, %%cr0" 
            :
            : "r"(cr0)
        );

        g_hh_enabled = true;
        g_pml4_v = reinterpret_cast<u64*>(g_pml4_p + HIGHER_HALF_OFFSET);
        //info->frameBufferBase = HIGHER_HALF_OFFSET + info->frameBufferBase;

        return KERNEL_SUCCESS;
    }

    void map_page(u64 virt, u64 phys, u64 flags)
    {
        if (g_pml4_v == nullptr) return;

        u64 pageFlags = flags | PF_P;
        u64* pdpt = ensure_table(g_pml4_v, PML4_IDX(virt));
        if (pdpt == nullptr) return;
        
        u64* pd = ensure_table(pdpt, PDPT_IDX(virt));
        if (pd == nullptr) return;

        u64* pt = ensure_table(pd, PD_IDX(virt));
        if (pt == nullptr) return;

        pt[PT_IDX(virt)] = (phys & PAGE_ADDR_MASK) | pageFlags;
    }

    void unmap_page(u64 virt)
    {
        if (g_pml4_v == nullptr) return;

        if (!(g_pml4_v[PML4_IDX(virt)] & PF_P)) return;

        u64* pdpt = phys2virt(g_pml4_v[PML4_IDX(virt)] & PAGE_ADDR_MASK);
        if (pdpt == nullptr) return;
        if (!(pdpt[PDPT_IDX(virt)] & PF_P)) return;

        u64* pd = phys2virt(pdpt[PDPT_IDX(virt)] & PAGE_ADDR_MASK);
        if (pd == nullptr) return;
        if (!(pd[PD_IDX(virt)] & PF_P)) return;

        u64* pt = phys2virt(pd[PD_IDX(virt)] & PAGE_ADDR_MASK);
        if (pt == nullptr) return;
        if (!(pt[PT_IDX(virt)] & PF_P)) return;

        pt[PT_IDX(virt)] = 0;

        __asm__ volatile("invlpg (%0)" :: "r"(virt) : "memory");
    }

    void map_range(u64 virt, u64 physStart, u64 size, u64 flags, u64 opts)
    {
        u64 end = virt + size;
        end  = (end + PAGE_OFFSET_MASK) & ~PAGE_OFFSET_MASK;
        size = end - virt;
        while (size)
        {
            bool huge = (opts & MAP_HUGE) && !(virt & 0x1FFFFFULL) && !(physStart & 0x1FFFFFULL) && size >= 0x200000;
            if (huge)
            {
                u64* pdpt = ensure_table(g_pml4_v, PML4_IDX(virt));
                if (pdpt == nullptr) return;
                u64* pd = ensure_table(pdpt, PDPT_IDX(virt));
                if (pd == nullptr) return;

                pd[PD_IDX(virt)] = (physStart & PHYS_MASK_2MIB) | flags | PF_P | PF_PS;
                virt += 0x200000;
                physStart += 0x200000;
                size -= 0x200000;
            }
            else
            {
                map_page(virt, physStart, flags);
                virt += PAGE_SIZE;
                physStart += PAGE_SIZE;
                size -= PAGE_SIZE;
            }
        }
    }

    void unmap_range(u64 virt, u64 size)
    {
        if (size == 0) return;

        u64 end = virt + size;
        virt &= ~PAGE_OFFSET_MASK;
        end = (end + PAGE_OFFSET_MASK) & ~PAGE_OFFSET_MASK;

        while (virt < end)
        {
            u64 pml4e = g_pml4_v[PML4_IDX(virt)];
            if (!(pml4e & PF_P))
            {
                virt = ((virt >> 39) + 1ULL) << 39;
                continue;
            }

            u64* pdpt = phys2virt(pml4e & PAGE_ADDR_MASK);
            u64 pdpte = pdpt[PDPT_IDX(virt)];
            if (!(pdpte & PF_P))
            {
                virt = ((virt >> 30) + 1ULL) << 30;
                continue;
            }

            u64* pd  = phys2virt(pdpte & PAGE_ADDR_MASK);
            u64 pde  = pd[PD_IDX(virt)];

            if (pde & PF_PS)
            {
                pd[PD_IDX(virt)] = 0;
                __asm__ volatile ("invlpg (%0)" :: "r"(virt) : "memory");
                virt += 0x200000;
                continue;
            }

            if (!(pde & PF_P))
            {
                virt = ((virt >> 21) + 1ULL) << 21;
                continue;
            }

            u64* pt  = phys2virt(pde & PAGE_ADDR_MASK);
            u64 pte  = pt[PT_IDX(virt)];

            if (pte & PF_P)
            {
                pt[PT_IDX(virt)] = 0;
                __asm__ volatile ("invlpg (%0)" :: "r"(virt) : "memory");
            }

            virt += PAGE_SIZE;
        }
    }

    u64 get_phys(u64 virt)
    {
        if (g_pml4_v == nullptr) return 0;
        if (!(g_pml4_v[PML4_IDX(virt)] & PF_P)) return 0;
        
        u64* pdpt = phys2virt(g_pml4_v[PML4_IDX(virt)] & PAGE_ADDR_MASK);
        if (pdpt == nullptr) return 0;
        if (!(pdpt[PDPT_IDX(virt)] & PF_P)) return 0;

        u64* pd = phys2virt(pdpt[PDPT_IDX(virt)] & PAGE_ADDR_MASK);
        if (pd == nullptr) return 0;
        u64 pde = pd[PD_IDX(virt)];
        if (!(pde & PF_P)) return 0;
        if (pde & PF_PS) return (pde & PHYS_MASK_2MIB) | (virt & 0x1FFFFFULL);
        
        u64* pt = phys2virt(pd[PD_IDX(virt)] & PAGE_ADDR_MASK);
        if (pt == nullptr) return 0;

        u64 e = pt[PT_IDX(virt)];
        if (!(e & PF_P)) return 0;

        return (e & PAGE_ADDR_MASK) | (virt & PAGE_OFFSET_MASK);
    }
}
