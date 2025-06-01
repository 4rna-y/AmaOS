#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/GraphicsOutput.h>
#include <Guid/FileInfo.h>
#include <Guid/Acpi.h>

#include <elf.h>

#include "FileUtility.h"
#include "GraphicsUtility.h"
#include "../Kernel/BootInfo.h"

EFI_STATUS LoadKernelElf(
    IN VOID*    elfBuffer,
    IN UINTN    elfSize,
    OUT VOID**  entryPoint
)
{
    EFI_STATUS status;
    Elf64_Ehdr *ehdr = elfBuffer;
    Elf64_Phdr *phdr = (VOID*)((UINT8*)elfBuffer + ehdr->e_phoff);
    UINT64 originalBase = UINT64_MAX;
    UINT64 highestEnd = 0;
    UINTN totalPages = 0;

    int ei0 = ehdr->e_ident[EI_MAG0] != ELFMAG0;
    int ei1 = ehdr->e_ident[EI_MAG1] != ELFMAG1;
    int ei2 = ehdr->e_ident[EI_MAG2] != ELFMAG2;
    int ei3 = ehdr->e_ident[EI_MAG3] != ELFMAG3;
    int eic = ehdr->e_ident[EI_CLASS] != ELFCLASS64;
    int eid = ehdr->e_ident[EI_DATA] != ELFDATA2LSB;
    int et = ehdr->e_type != ET_EXEC;
    int em = ehdr->e_machine != EM_X86_64;

    if (ei0 || ei1 || ei2 || ei3 || eic || eid || et || em) return EFI_UNSUPPORTED;

    for (UINTN i = 0; i < ehdr->e_phnum; i++) 
    {
        if (phdr[i].p_type != PT_LOAD) continue;
        if (phdr[i].p_paddr < originalBase)
        {
            originalBase = phdr[i].p_paddr;
        }

        UINT64 end = phdr[i].p_paddr + phdr[i].p_memsz;
        if (end > highestEnd) highestEnd = end;
    }

    if (originalBase == UINT64_MAX) return EFI_INVALID_PARAMETER;


    for (UINTN i = 0; i < ehdr->e_phnum; i++) 
    {
        if (phdr[i].p_type != PT_LOAD) continue;
        UINT64 start = phdr[i].p_paddr & ~(EFI_PAGE_SIZE - 1);
        UINT64 end = (phdr[i].p_paddr + phdr[i].p_memsz + EFI_PAGE_SIZE - 1) & ~(EFI_PAGE_SIZE - 1);

        totalPages += (UINTN)((end - start) / EFI_PAGE_SIZE);
    }

    EFI_PHYSICAL_ADDRESS loadBase = 0;
    status = gBS->AllocatePages(AllocateAnyPages, EfiLoaderData, totalPages, &loadBase);
    if (EFI_ERROR(status)) return status;

    for (UINTN i = 0; i < ehdr->e_phnum; i++)
    {
        if (phdr[i].p_type != PT_LOAD) continue;
        UINT64 offset = phdr[i].p_paddr - originalBase;
        UINT8* dest = (UINT8*)(UINTN)(loadBase + offset);

        CopyMem(dest, (UINT8*)elfBuffer + phdr[i].p_offset, phdr[i].p_filesz);
        SetMem(dest + phdr[i].p_filesz, phdr[i].p_memsz - phdr[i].p_filesz, 0);
    }

    *entryPoint = (VOID*)(UINTN)((ehdr->e_entry - originalBase) + loadBase);

    return EFI_SUCCESS;
}

EFI_STATUS GetMemoryMap(
    OUT EFI_MEMORY_DESCRIPTOR**  memMap,
    OUT UINTN*  mapSize,
    OUT UINTN*  mapKey,
    OUT UINTN*  descSize,
    OUT UINT32* descVersion
)
{
    EFI_STATUS status;
    EFI_MEMORY_DESCRIPTOR dummy;
    *mapSize = 0;
    *mapKey = 0;
    *descSize = 0;
    *descVersion = 0;
    *memMap = NULL;

    status = gBS->GetMemoryMap(mapSize, &dummy, mapKey, descSize, descVersion);
    if (status != EFI_BUFFER_TOO_SMALL) return status;

    *memMap = AllocatePool(*mapSize);
    if (!*memMap) return EFI_OUT_OF_RESOURCES;
    ZeroMem(*memMap, *mapSize);

    status = gBS->GetMemoryMap(mapSize, *memMap, mapKey, descSize, descVersion);
    if (EFI_ERROR(status)) 
    {
        FreePool(*memMap);
        *memMap = NULL;
    }
    
    return status;
}

EFI_STATUS GetRootSystemDescriptionPointerAddress(
    IN EFI_SYSTEM_TABLE* systemTable,
    OUT UINT64* rootSystemDescriptionPointer
)
{
    EFI_STATUS status = EFI_NOT_FOUND;
    *rootSystemDescriptionPointer = 0;

    for (UINTN i = 0; i < systemTable->NumberOfTableEntries; i++)
    {
        if (CompareGuid(&systemTable->ConfigurationTable[i].VendorGuid, &gEfiAcpi20TableGuid) ||
            CompareGuid(&systemTable->ConfigurationTable[i].VendorGuid, &gEfiAcpi10TableGuid))
        {
            *rootSystemDescriptionPointer = (UINT64)(UINTN)systemTable->ConfigurationTable[i].VendorTable;
            return EFI_SUCCESS;
        }
    }

    return status;
}

EFI_STATUS GetCommandLine(
    IN EFI_HANDLE imageHandle,
    OUT CHAR8** commandLine
)
{
    EFI_STATUS status;
    EFI_LOADED_IMAGE_PROTOCOL* loadedImage = NULL;
    *commandLine = NULL;
    
    status = gBS->HandleProtocol(imageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&loadedImage);
    if (!EFI_ERROR(status) && loadedImage->LoadOptionsSize > 0)
    {
        *commandLine = (CHAR8*)(UINTN)loadedImage->LoadOptions;
    }

    return status;
}

void Halt() 
{
    while (1) __asm__("hlt");
}

void Panic()
{

}

EFI_STATUS EFIAPI UefiMain(
    IN EFI_HANDLE imageHandle,
    IN EFI_SYSTEM_TABLE* systemTable
) 
{
    systemTable->ConOut->ClearScreen(systemTable->ConOut);

    EFI_STATUS status;

    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop = NULL;

    LOADER_DISPLAY_MODE mode = {0};
    VOID* elfBuffer = NULL;
    UINTN elfSize = 0;
    VOID* kernelEntry = NULL;
    EFI_MEMORY_DESCRIPTOR* memMap = NULL;
    UINTN mapSize = 0;
    UINTN mapKey = 0;
    UINTN descSize = 0;
    UINT32 descVersion = 0;
    UINT64 frameBufferBase = 0;
    UINT64 frameBufferSize = 0;
    UINT64 rootSystemDescriptionPointerAddress = 0;
    CHAR8* commandLine = NULL;
    BootInfo info = {0};

    status = InitializeGraphics(&gop);
    if (EFI_ERROR(status))
    {
        Print(L"[ERROR] InitializeGraphics : %r\n", status);
        Halt();
    }

    status = GetNativeDisplayInfo(gop, &mode);
    if (EFI_ERROR(status))
    {
        Print(L"[Warning] EDID was not found. Run with default resolution.\n");
    }
    else
    {
        status = SetDisplayMode(gop, &mode);
        if (EFI_ERROR(status))
        {
            Print(L"[ERROR] SetDisplayMode : %r\n", status);
            Halt();
        }
    }

    status = GetDisplayMode(gop, &frameBufferBase, &frameBufferSize, &mode);
    if (EFI_ERROR(status))
    {
        Print(L"[ERROR] GetDisplayMode : %r\n", status);
        Halt();
    }

    status = ReadFileToBuffer(imageHandle, L"amaos\\kernel.elf", &elfBuffer, &elfSize);
    if (EFI_ERROR(status))
    {
        Print(L"[ERROR] ReadKernelElf : %r\n", status);
        Halt();
    }

    status = LoadKernelElf(elfBuffer, elfSize, &kernelEntry);
    FreePool(elfBuffer);
    elfBuffer = NULL;
    if (EFI_ERROR(status))
    {
        Print(L"[ERROR] LoadKernelElf : %r\n", status);
        Halt();
    }

    status = GetMemoryMap(&memMap, &mapSize, &mapKey, &descSize, &descVersion);
    if (EFI_ERROR(status))
    {
        Print(L"[ERROR] GetMemoryMap : %r\n", status);
        Halt();
    }

    status = GetRootSystemDescriptionPointerAddress(systemTable, &rootSystemDescriptionPointerAddress);
    if (EFI_ERROR(status))
    {
        Print(L"[ERROR] GetRootSystemDescriptionPointerAddress : %r\n", status);
        Halt();
    }

    status = GetCommandLine(imageHandle, &commandLine);
    if (EFI_ERROR(status))
    {
        Print(L"[ERROR] GetCommandLine : %r\n", status);
        Halt();
    }

    info.memoryMap = memMap;
    info.memoryMapSize = mapSize;
    info.mapDescriptorSize = descSize;
    info.mapDescriptorVersion = descVersion;
    info.frameBufferBase = (unsigned long long)frameBufferBase;
    info.frameBufferSize = (unsigned long long)frameBufferSize;
    info.frameBufferWidth = mode.width;
    info.frameBufferHeight = mode.height;
    info.rootSystemDescriptionPointerAddress = (unsigned long long)rootSystemDescriptionPointerAddress;
    info.cmdLine = commandLine;

    status = gBS->ExitBootServices(imageHandle, mapKey);
    if (EFI_ERROR(status))
    {
        Print(L"[ERROR] ExitBootServices : %r\n", status);
        Halt();
    }

    ((void (*)(BootInfo*))kernelEntry)(&info);

    Print(L"DOUYATTE OMAE HA KOKO NI TADORI TUITA??\n");
    UNREACHABLE();

    return EFI_SUCCESS;
}
