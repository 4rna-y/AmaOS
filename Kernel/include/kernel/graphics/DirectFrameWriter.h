#pragma once

#include <stdint.h>

#include "LoaderBootInfo.h"
#include "KernelStatus.h"
#include "Point.h"
#include "Color.h"
#include "FrameBufferInfo.h"
#include "StandardCharactorBitmapProvider.h"

namespace dfw
{
    KERNEL_STATUS init(LOADER_BOOT_INFO* info);
    KERNEL_STATUS switchHigherHalf();
    KERNEL_STATUS clear();
    KERNEL_STATUS draw_pixel(POINT point, COLOR color);
    KERNEL_STATUS draw_box(POINT point, SIZE size, COLOR color);
    KERNEL_STATUS draw_char(POINT point, SIZE_F scale, COLOR color, char c);
    KERNEL_STATUS draw_string_f(POINT point, SIZE_F scale, COLOR color, const char* fmt, ...);
}