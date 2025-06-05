#pragma once

#include <stdint.h>

#include "Size.h"

typedef struct _frame_buffer_info
{
    uint8_t* base;
    uint64_t size;
    uint32_t pixelsPerScanLine;
    uint32_t bytesPerPixel;
    uint8_t isBGR;
    SIZE frameSize;
} FRAME_BUFFER_INFO;