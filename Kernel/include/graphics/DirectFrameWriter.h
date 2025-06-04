#pragma once

#include <stdint.h>

#include "KernelStatus.h"
#include "Point.h"
#include "Color.h"
#include "FrameBufferInfo.h"

KERNEL_STATUS k_dfw_init(FRAME_BUFFER_INFO* info);
KERNEL_STATUS k_dfw_draw_pixel();
KERNEL_STATUS k_dfw_draw_box(POINT point, SIZE size, COLOR color);