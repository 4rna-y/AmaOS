#pragma once

#include <stdint.h>

#include "KernelStatus.h"

#define CHARACTOR_BITMAP_HEIGHT 8

const uint8_t* get_char_bitmap(char c);
