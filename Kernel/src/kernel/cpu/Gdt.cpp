#include "cpu/Gdt.h"
#include "debug/Log.h"

alignas(16) static u64 g_gdt[7];
static GDT_POINTER g_ptr;
static TSS64 g_tss;

static constexpr u64 create_code_desc()
{
    return (0x00000000000000ULL) | 
        (0xAULL << 40) | 
        (1ULL << 44) |
        (1ULL << 47) |
        (1ULL << 53) |
        (1ULL << 55);
}

static constexpr u64 create_data_desc()
{
    const uint32_t limit = 0xFFFFF;       // 4 GiB −1 (G=1 で ×4 KiB)
    return
        /* Limit[15:0]  */  (uint64_t)(limit & 0xFFFF)      |
        /* Base  [15:0] */  0ULL            << 16           |
        /* Base  [23:16]*/  0ULL            << 32           |
        /* Type(2) S P  */  (0x92ULL)       << 40           | // RW + Present
        /* Limit[19:16] */  (uint64_t)((limit >> 16) & 0xF) << 48 |
        /* G=1 L=0 D=0  */  (1ULL << 55);  
}

static u64 create_tss_desc_low(u64 base)
{
    uint32_t lim = sizeof(TSS64) - 1;
    return   ((uint64_t)lim & 0xFFFF)          |
            ((uint64_t)base & 0xFFFF) << 16    |
            ((uint64_t)(base>>16)&0xFF) << 32  |
            (0x89ULL              ) << 40      | 
            ((uint64_t)lim>>16 &0xF)<<48       |
            (1ULL<<55); 
}

static u64 create_tss_desc_high(u64 base)
{
    return (base >> 32) & 0xFFFFFFFFULL;
}

namespace gdt
{
    void build_table()
    {
        g_gdt[0] = 0;
        g_gdt[1] = create_code_desc();
        g_gdt[2] = create_data_desc();
        g_gdt[3] = create_tss_desc_low(reinterpret_cast<u64>(&g_tss));
        g_gdt[4] = create_tss_desc_high(reinterpret_cast<u64>(&g_tss));
        g_gdt[5] = create_code_desc();
        g_gdt[6] = create_data_desc();
        g_ptr.limit = sizeof(g_gdt) - 1;
        g_ptr.base = reinterpret_cast<u64>(g_gdt); 

        //debug::print((u64)(g_gdt), true);
    }

    const GDT_POINTER* ptr() { return &g_ptr; }

    const void* tss_base() { return &g_tss;}
}
