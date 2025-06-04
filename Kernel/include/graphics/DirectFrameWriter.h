#pragma once

#include <stdint.h>

#include "KernelStatus.h"
#include "Point.h"
#include "Color.h"
#include "FrameBufferInfo.h"

KERNEL_STATUS k_dfw_draw_box(FRAME_BUFFER_INFO* info, POINT point, SIZE size, COLOR color);