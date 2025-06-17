#include "cpu/Tss.h"

alignas(16) u8 g_kernel_stack[16 * 1024];
alignas(16) u8 g_ist_df_stack[8 * 1024];
alignas(16) u8 g_ist_pf_stack[8 * 1024];

TSS64& get_tss() { return *reinterpret_cast<TSS64*>(const_cast<void*>(gdt::tss_base())); }

namespace tss
{
    void init()
    {
        get_tss().rsp0 = reinterpret_cast<u64>(g_kernel_stack + sizeof(g_kernel_stack));
        get_tss().ist1 = reinterpret_cast<u64>(g_ist_df_stack + sizeof(g_ist_df_stack));
        get_tss().ist2 = reinterpret_cast<u64>(g_ist_pf_stack + sizeof(g_ist_pf_stack));
        get_tss().iopbOffset = sizeof(TSS64);
    }
}