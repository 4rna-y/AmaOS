#include "FileUtility.h"

#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include <Guid/FileInfo.h>

EFI_STATUS ReadFileToBuffer(
    IN EFI_HANDLE   imageHandle,
    IN CHAR16*      filePath,
    OUT VOID**      buffer,
    OUT UINTN*      size
)
{
    EFI_STATUS                          status;
    EFI_LOADED_IMAGE_PROTOCOL*          loadedImage     = NULL;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL*    fileSystem      = NULL;
    EFI_FILE_PROTOCOL*                  root            = NULL;
    EFI_FILE_PROTOCOL*                  file      = NULL;
    EFI_FILE_INFO*                      fileInfo        = NULL;
    UINTN                               fileInfoSize    = 0;
    VOID*                               tmpBuffer       = NULL;

    *buffer = NULL;
    *size = 0;

    status = gBS->HandleProtocol(imageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&loadedImage);
    if (EFI_ERROR(status)) goto Exit;

    status = gBS->HandleProtocol(loadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID**)&fileSystem);
    if (EFI_ERROR(status)) goto Exit;

    status = fileSystem->OpenVolume(fileSystem, &root);
    if (EFI_ERROR(status)) goto Exit;

    status = root->Open(root, &file, filePath, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(status)) goto Exit;

    status = file->GetInfo(file, &gEfiFileInfoGuid, &fileInfoSize, NULL);
    if (status != EFI_BUFFER_TOO_SMALL) goto Exit;

    fileInfo = AllocatePool(fileInfoSize);
    if (fileInfo == NULL)
    {
        status = EFI_OUT_OF_RESOURCES;
        goto Exit;
    }

    status = file->GetInfo(file, &gEfiFileInfoGuid, &fileInfoSize, fileInfo);
    if (EFI_ERROR(status)) goto Exit;

    *size = fileInfo->FileSize;
    FreePool(fileInfo);
    fileInfo = NULL;

    status = gBS->AllocatePool(EfiLoaderData, *size, &tmpBuffer);
    if (EFI_ERROR(status)) goto Exit;

    UINTN readSize = *size;
    status = file->Read(file, &readSize, tmpBuffer);
    if (EFI_ERROR(status)) goto Exit;

    *buffer = tmpBuffer;
    tmpBuffer = NULL;

Exit:
    if (file != NULL)
    {
        file->Close(file);
    }
    if (root != NULL)
    {
        root->Close(root);
    }
    if (fileInfo != NULL)
    {
        FreePool(fileInfo);
    }
    if (EFI_ERROR(status) && tmpBuffer != NULL)
    {
        FreePool(tmpBuffer);
    }

    return status;
}