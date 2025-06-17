#pragma once
#include "Types.h"

struct PANIC_INFO
{
    const char* reason;
    u64 faultAddr;
    u64 errorCode;
    u64 rip;
    u64 registers[16];
};

struct INTERRUPT_FRAME
{
    u64 rip;
    u64 cs;
    u64 rflags;
    u64 rsp;
    u64 ss;
};
