#ifndef FILE_UTILITY_H
#define FILE_UTILITY_H

#include <Uefi.h>

EFI_STATUS ReadFileToBuffer(
    IN EFI_HANDLE   imageHandle,
    IN CHAR16*      filePath,
    OUT VOID**      buffer,
    OUT UINTN*      size
);

#endif