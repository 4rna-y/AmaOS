#pragma once

#include "Types.h"
#include <stddef.h>

namespace memop
{
    void* set_mem(void* dest, size_t len, int value);
    void* zero_mem(void* dest, size_t len);
}