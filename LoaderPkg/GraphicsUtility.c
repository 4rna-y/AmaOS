#include "GraphicsUtility.h"
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>

EFI_STATUS InitializeGraphics(
    OUT EFI_GRAPHICS_OUTPUT_PROTOCOL** gop
)
{
    EFI_STATUS status;
    if (gop == NULL) return EFI_INVALID_PARAMETER;

    status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)gop);
    
    return status;
}

EFI_STATUS GetNativeDisplayInfo(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
    OUT LOADER_DISPLAY_MODE* diplayMode
)
{
    EFI_STATUS status;
    EFI_EDID_DISCOVERED_PROTOCOL* edidp = NULL;
    UINT8* edidData = NULL;
    UINT32 edidSize = 0;
    UINT32 nativeWidth = 0;
    UINT32 nativeHeight = 0;
    UINTN maxMode = gop->Mode->MaxMode;
    UINTN infoSize = 0;
    UINT32 found = (UINT32)-1;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* info = NULL;

    status = gBS->LocateProtocol(&gEfiEdidDiscoveredProtocolGuid, NULL, (VOID**)&edidp);
    if (EFI_ERROR(status) || edidp == NULL) return EFI_NOT_FOUND;

    edidData = edidp->Edid;
    edidSize = edidp->SizeOfEdid;

    if (edidData == NULL || edidSize < EDID_MIN_SIZE) return EFI_DEVICE_ERROR;

    status = EFI_NOT_FOUND;
    for (UINTN offset = EDID_DTD_OFFSET; offset + EDID_DTD_BLOCK_SIZE <= edidSize; offset += EDID_DTD_BLOCK_SIZE)
    {
        UINT16 pixelClock = *(UINT16*)&edidData[offset];
        if (pixelClock == 0) continue;

        UINT8 horActiveLSB = edidData[offset + 2];
        UINT8 horActiveMSB = (edidData[offset + 4] & 0xF0) >> 4;
        UINT32 horActive = ((UINT32)horActiveMSB << 8) | horActiveLSB;
        UINT8 verActiveLSB = edidData[offset + 5];
        UINT8 verActiveMSB = (edidData[offset + 7] & 0xF0) >> 4;
        UINT32 verActive = ((UINT32)verActiveMSB << 8) | verActiveLSB;

        if (horActive == 0 || verActive == 0) continue;

        nativeWidth = horActive;
        nativeHeight = verActive;
        status = EFI_SUCCESS;

        break;
    }

    if (EFI_ERROR(status)) return status;

    for (UINT32 i = 0; i < maxMode; i++)
    {
        status = gop->QueryMode(gop, i, &infoSize, &info);
        if (EFI_ERROR(status)) continue;
        if (info->HorizontalResolution == nativeWidth &&
            info->VerticalResolution == nativeHeight)
        {
            found = i;
            break;
        }
    }

    if (found == (UINT32)-1) return EFI_UNSUPPORTED;

    diplayMode->index = found;
    diplayMode->width = nativeWidth;
    diplayMode->height = nativeHeight;
    diplayMode->pixelFormat = info->PixelFormat;
    
    return EFI_SUCCESS;
}

EFI_STATUS SetDisplayMode(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
    IN LOADER_DISPLAY_MODE* mode
)
{
    if (gop == NULL || mode == NULL) return EFI_INVALID_PARAMETER;

    return gop->SetMode(gop, mode->index);
}

EFI_STATUS GetDisplayMode(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
    OUT UINT64* frameBufferBase,
    OUT UINT64* frameBufferSize,
    OUT LOADER_DISPLAY_MODE* mode
)
{
    if (gop == NULL || mode == NULL) return EFI_INVALID_PARAMETER;
    if (gop->Mode == NULL || gop->Mode->Info == NULL) return EFI_UNSUPPORTED;

    *frameBufferBase = gop->Mode->FrameBufferBase;
    *frameBufferSize = gop->Mode->FrameBufferSize;
    mode->index = gop->Mode->Mode;
    mode->width = gop->Mode->Info->HorizontalResolution;
    mode->height = gop->Mode->Info->VerticalResolution;
    mode->pixelFormat = gop->Mode->Info->PixelFormat;
    mode->pixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;

    return EFI_SUCCESS;
}