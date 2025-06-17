#pragma once
#include "Types.h"
#include "cpu/Gdt.h"

struct __attribute__((packed)) TSS64
{
    uint32_t reserved0 = 0;
    uint64_t rsp0      = 0;
    uint64_t rsp1      = 0;
    uint64_t rsp2      = 0;

    uint64_t reserved1 = 0;
    uint64_t ist1      = 0;
    uint64_t ist2      = 0;
    uint64_t ist3      = 0;
    uint64_t ist4      = 0;
    uint64_t ist5      = 0;
    uint64_t ist6      = 0;
    uint64_t ist7      = 0;

    uint64_t reserved2 = 0;
    uint16_t reserved3 = 0;
    uint16_t iopbOffset = 0;
};
static_assert(sizeof(TSS64) == 0x68, "TSS64 must be 104 bytes");

namespace tss
{
    void init();
}