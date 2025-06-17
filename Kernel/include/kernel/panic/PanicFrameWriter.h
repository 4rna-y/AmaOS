#pragma once

#include "Types.h"
#include "LoaderBootInfo.h"
#include "panic/PanicInfo.h"

namespace panic
{
    void init(const LOADER_BOOT_INFO* info) noexcept;
    
    __attribute__((no_caller_saved_registers))
    void panic(const PANIC_INFO& info) noexcept;
    
    __attribute__((no_caller_saved_registers))
    void clear(u8 red, u8 green, u8 blue) noexcept;

    __attribute__((no_caller_saved_registers))
    void draw_string(u32 x, u32 y, const char* str) noexcept; 
}