#pragma once
#include "Types.h"
#include "memory/MemoryOperator.h"
#include "cpu/Tss.h"

struct Tss64;

struct __attribute__((packed)) GDT_POINTER
{
    u16 limit;
    u64 base;
};

namespace gdt
{
    void build_table();
    const GDT_POINTER* ptr();
    const void* tss_base();
}



