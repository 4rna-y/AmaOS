#pragma once

#include "Types.h"

#include "KernelStatus.h"

namespace stdcharbmp
{
    #define CHARACTOR_BITMAP_HEIGHT 8

    void init() noexcept;

    const u8* get_char_bitmap(char c) noexcept;
}
