#include "graphics/DirectFrameWriter.h"

static FRAME_BUFFER_INFO* g_fb_info = nullptr;

KERNEL_STATUS k_dfw_init(FRAME_BUFFER_INFO* info)
{
    if (g_fb_info != nullptr) return KERNEL_FAILURE;
    g_fb_info = info;

    return KERNEL_SUCCESS;
}

KERNEL_STATUS k_dfw_clear()
{
    if (g_fb_info == nullptr) return KERNEL_FAILURE;
    for (uint64_t i = 0; i < g_fb_info->size; i++)
    {
        g_fb_info->base[i] = 0;
    }

    return KERNEL_SUCCESS;
}

KERNEL_STATUS k_dfw_draw_pixel(POINT point, COLOR color)
{
    if (g_fb_info == nullptr) return KERNEL_FAILURE;

    if (point.x >= g_fb_info->frameSize.width || point.y >= g_fb_info->frameSize.height) 
        return KERNEL_FAILURE;

    uint64_t bufferIdx = (uint64_t)(point.x + point.y * g_fb_info->pixelsPerScanLine) * g_fb_info->bytesPerPixel;

    if (g_fb_info->isBGR) 
    {
        g_fb_info->base[bufferIdx + 0] = color.blue;
        g_fb_info->base[bufferIdx + 1] = color.green;
        g_fb_info->base[bufferIdx + 2] = color.red;
        g_fb_info->base[bufferIdx + 3] = 0;
    } 
    else 
    {
        g_fb_info->base[bufferIdx + 0] = color.red;
        g_fb_info->base[bufferIdx + 1] = color.green;
        g_fb_info->base[bufferIdx + 2] = color.blue;
        g_fb_info->base[bufferIdx + 3] = 0;
    }

    return KERNEL_SUCCESS;
}

KERNEL_STATUS k_dfw_draw_box(POINT point, SIZE size, COLOR color)
{
    if (g_fb_info == nullptr) return KERNEL_FAILURE;

    if (point.x >= g_fb_info->frameSize.width || point.y >= g_fb_info->frameSize.height) 
        return KERNEL_FAILURE;

    if (size.width == 0 || size.height == 0) return KERNEL_FAILURE;

    if (point.x + size.width  > g_fb_info->frameSize.width || point.y + size.height > g_fb_info->frameSize.height) 
        return KERNEL_FAILURE;

    uint32_t pitchPixels = g_fb_info->pixelsPerScanLine;
    uint32_t bpp = g_fb_info->bytesPerPixel;
    uint64_t fbBytes = g_fb_info->size;

    for (uint32_t y = 0; y < size.height; ++y) 
    {
        for (uint32_t x = 0; x < size.width; ++x) 
        {
            uint32_t dx = point.x + x;
            uint32_t dy = point.y + y;

            uint64_t pixelIndex = (uint64_t)dy * pitchPixels + dx;
            uint64_t bufferIdx  = pixelIndex * bpp;

            if (bufferIdx + bpp - 1 >= fbBytes) continue;
            
            if (g_fb_info->isBGR) 
            {
                g_fb_info->base[bufferIdx + 0] = color.blue;
                g_fb_info->base[bufferIdx + 1] = color.green;
                g_fb_info->base[bufferIdx + 2] = color.red;
                g_fb_info->base[bufferIdx + 3] = 0;
            } 
            else 
            {
                g_fb_info->base[bufferIdx + 0] = color.red;
                g_fb_info->base[bufferIdx + 1] = color.green;
                g_fb_info->base[bufferIdx + 2] = color.blue;
                g_fb_info->base[bufferIdx + 3] = 0;
            }
        }
    }

    return KERNEL_SUCCESS;
}

KERNEL_STATUS k_dfw_draw_str_f(POINT point, float scale, COLOR color, const char* fmt, ...)
{
    
}