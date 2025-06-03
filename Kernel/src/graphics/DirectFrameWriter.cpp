#include "graphics/DirectFrameWriter.h"

KERNEL_STATUS k_dfw_draw_box(FRAME_BUFFER_INFO* info,
                             POINT           point,
                             SIZE            size,
                             COLOR           color)
{

    if (point.x >= info->frameSize.width ||
        point.y >= info->frameSize.height)
    {
        return KERNEL_FAILURE;
    }

    if (size.width == 0 || size.height == 0) {
        return KERNEL_FAILURE;
    }

    if (point.x + size.width  > info->frameSize.width ||
        point.y + size.height > info->frameSize.height)
    {
        return KERNEL_FAILURE;
    }

    uint32_t pitchPixels = info->pixelsPerScanLine;
    uint32_t bpp = info->bytesPerPixel;
    uint64_t fbBytes = info->size;

    for (uint32_t y = 0; y < size.height; ++y) {
        for (uint32_t x = 0; x < size.width; ++x) {
            uint32_t dx = point.x + x;
            uint32_t dy = point.y + y;

            uint64_t pixelIndex = (uint64_t)dy * pitchPixels + dx;
            uint64_t bufferIdx  = pixelIndex * bpp;

            if (bufferIdx + bpp - 1 >= fbBytes) continue;
            
            if (info->isBGR) 
            {
                info->base[bufferIdx + 0] = color.blue;
                info->base[bufferIdx + 1] = color.green;
                info->base[bufferIdx + 2] = color.red;
                info->base[bufferIdx + 3] = 0;
            } 
            else 
            {
                info->base[bufferIdx + 0] = color.red;
                info->base[bufferIdx + 1] = color.green;
                info->base[bufferIdx + 2] = color.blue;
                info->base[bufferIdx + 3] = 0;
            }
        }
    }

    return KERNEL_SUCCESS;
}