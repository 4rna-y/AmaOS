#pragma once

#include <stdint.h>

#include "KernelStatus.h"
#include "Point.h"
#include "Color.h"
#include "FrameBufferInfo.h"
#include "StandardCharactorBitmapProvider.h"

KERNEL_STATUS k_dfw_init(FRAME_BUFFER_INFO* info);
KERNEL_STATUS k_dfw_clear();
KERNEL_STATUS k_dfw_draw_pixel(POINT point, COLOR color);
KERNEL_STATUS k_dfw_draw_box(POINT point, SIZE size, COLOR color);
KERNEL_STATUS k_dfw_draw_char(POINT point, float scale, COLOR color, char c);
KERNEL_STATUS k_dfw_draw_string_f(POINT point, float scale, COLOR color, const char* fmt, ...);