#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include "../Kernel/BootInfo.h"

EFI_STATUS ReadFileToBuffer(
    IN EFI_HANDLE imageHandle,
    IN CHAR16* filePath,
    OUT VOID** buffer,
    OUT UINTN* bufferSize
);

EFI_STATUS LoadElfImage(
    IN VOID* elfBuffer,
    IN UINTN elfSize,
    OUT EFI_PHYSICAL_ADDRESS* kernelBase,
    OUT VOID** entryPoint
);

EFI_STATUS EFIAPI UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
) 
{
    Print(L"Hello, UEFI World!\n");

    for (;;) {}

    return EFI_SUCCESS;
}
