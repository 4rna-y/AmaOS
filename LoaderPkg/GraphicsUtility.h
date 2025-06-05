#ifndef GRAPHICS_UTILITY_H
#define GRAPHICS_UTILITY_H

#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/EdidDiscovered.h>

#define EDID_DTD_OFFSET         54
#define EDID_DTD_BLOCK_SIZE     18
#define EDID_MIN_SIZE           128

typedef struct _loader_display_mode
{
    UINT32 index;
    UINT32 width;
    UINT32 height;
    EFI_GRAPHICS_PIXEL_FORMAT pixelFormat;
    UINT32 pixelsPerScanLine;
} LOADER_DISPLAY_MODE;


EFI_STATUS InitializeGraphics(
    OUT EFI_GRAPHICS_OUTPUT_PROTOCOL** gop
);

EFI_STATUS GetNativeDisplayInfo(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
    OUT LOADER_DISPLAY_MODE* diplayMode
);

EFI_STATUS SetDisplayMode(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
    IN LOADER_DISPLAY_MODE* mode
);

EFI_STATUS GetDisplayMode(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
    OUT UINT64* frameBufferBase,
    OUT UINT64* frameBufferSize,
    OUT LOADER_DISPLAY_MODE* mode
);

#endif