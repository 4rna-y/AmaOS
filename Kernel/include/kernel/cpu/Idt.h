#pragma once
#include "Types.h"
#include "panic/PanicInfo.h"


struct __attribute__((packed)) IDT_ENTRY
{
    u16 offsetLow;
    u16 selector;
    u8 ist : 3;
    u8 zero : 5;
    u8 typeAttr;
    u16 offsetMid;
    u32 offsetHigh;
    u32 zero2;
} ;

struct __attribute__((packed)) IDT_POINTER
{
    u16 limit;
    u64 base;
};

namespace idt
{
    void build_exceptions();
    const IDT_POINTER* ptr();
}