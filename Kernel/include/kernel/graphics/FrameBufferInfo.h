#pragma once

#include "Types.h"
#include "Size.h"

typedef struct _frame_buffer_info
{
    u8* base;
    u64 size;
    u32 pixelsPerScanLine;
    u32 bytesPerPixel;
    u8 isBGR;
    SIZE frameSize;
} FRAME_BUFFER_INFO;