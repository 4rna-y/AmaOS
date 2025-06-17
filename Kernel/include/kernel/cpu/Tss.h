#pragma once
#include "Types.h"
#include "cpu/Gdt.h"

struct alignas(16) __attribute__ ((packed)) TSS64
{
    u32 reserved0 = 0;
    u64 rsp0 = 0;
    u64 rsp1 = 0;
    u64 rsp2 = 0;
    u64 reserved1 = 0;
    u64 ist1 = 0;
    u64 ist2 = 0;
    u64 ist3 = 0;
    u64 ist4 = 0;
    u64 ist5 = 0;
    u64 ist6 = 0;
    u64 ist7 = 0;
    u64 reserved2 = 0;
    u16 reserved3 = 0;
    u16 iopbOffset = sizeof(TSS64);
};

namespace tss
{
    void init();
}