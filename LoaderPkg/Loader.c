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
#include "../Kernel/LoaderBootInfo.h"

#define PAGE_SIZE   4096ULL

#define R_X86_64_RELATIVE 8
#define R_X86_64_64 1

#define BITMAP_SET(b, idx)   ((b)[(idx) >> 3] |=  (1U << ((idx) & 7)))
#define BITMAP_CLR(b, idx)   ((b)[(idx) >> 3] &= ~(1U << ((idx) & 7)))

EFI_STATUS LoadKernelElf(
    IN VOID*    elfBuffer,
    IN UINTN    elfSize,
    OUT EFI_PHYSICAL_ADDRESS* kernelBase,
    OUT UINTN* kernelBaseSize,
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
    int et  = !(ehdr->e_type == ET_EXEC || ehdr->e_type == ET_DYN);
    int em  = ehdr->e_machine != EM_X86_64;

    if (ei0 || ei1 || ei2 || ei3 || eic || eid || et || em) return EFI_UNSUPPORTED;

    for (UINTN i = 0; i < ehdr->e_phnum; i++) 
    {
        if (phdr[i].p_type != PT_LOAD) continue;
        UINT64 vstart = phdr[i].p_vaddr;
        UINT64 vend = phdr[i].p_vaddr + phdr[i].p_memsz;
        if (vstart < originalBase) originalBase = vstart;
        if (vend > highestEnd) highestEnd = vend;
    }

    if (originalBase == UINT64_MAX) return EFI_INVALID_PARAMETER;


    for (UINTN i = 0; i < ehdr->e_phnum; i++) 
    {
        if (phdr[i].p_type != PT_LOAD) continue;
        UINT64 start = phdr[i].p_vaddr & ~(EFI_PAGE_SIZE - 1);
        UINT64 end = (phdr[i].p_vaddr + phdr[i].p_memsz + EFI_PAGE_SIZE - 1) & ~(EFI_PAGE_SIZE - 1);

        totalPages += (UINTN)((end - start) / EFI_PAGE_SIZE);
    }

    EFI_PHYSICAL_ADDRESS loadBase = 0;
    status = gBS->AllocatePages(AllocateAnyPages, EfiLoaderData, totalPages, &loadBase);
    if (EFI_ERROR(status)) return status;

    for (UINTN i = 0; i < ehdr->e_phnum; i++)
    {
        if (phdr[i].p_type != PT_LOAD) continue;
        UINT64 offset = phdr[i].p_vaddr - originalBase;
        UINT8* dest = (UINT8*)(UINTN)(loadBase + offset);

        CopyMem(dest, (UINT8*)elfBuffer + phdr[i].p_offset, phdr[i].p_filesz);
        SetMem(dest + phdr[i].p_filesz, phdr[i].p_memsz - phdr[i].p_filesz, 0);
    }

    Elf64_Dyn* dyn = NULL;
    for (UINTN i = 0; i < ehdr->e_phnum; i++)
    {
        if (phdr[i].p_type == PT_DYNAMIC)
        {
            dyn = (Elf64_Dyn*)(UINTN)(loadBase + phdr[i].p_vaddr);
            break;
        }
    }

    if (dyn)
    {
        Elf64_Rela* rela = NULL;
        UINTN rela_count = 0;

        for (Elf64_Dyn* d = dyn; d->d_tag != DT_NULL; d++)
        {
            if (d->d_tag == DT_RELA)
            {
                rela = (Elf64_Rela*)(UINTN)(loadBase + d->d_un.d_ptr);
            }
            else
            if (d->d_tag == DT_RELASZ)
            {
                rela_count = d->d_un.d_val / sizeof(Elf64_Rela);
            }
        }

        for (UINTN i = 0; i < rela_count; i++)
        {
            Elf64_Rela* r = &rela[i];
            UINT64* where = (UINT64*)(UINTN)(loadBase + r->r_offset);
            UINT32 type = ELF64_R_TYPE(r->r_info);
            
            switch (type)
            {
                case R_X86_64_RELATIVE:
                    *where = loadBase + r->r_addend;
                    break;
                case R_X86_64_64:
                    *where = loadBase + r->r_addend;
                    break;
                default:
                    break;
            }
        }
    }

    *kernelBase = loadBase;
    *kernelBaseSize = highestEnd - originalBase;
    *entryPoint = (VOID*)(UINTN)(loadBase + ehdr->e_entry);

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

static VOID SetUsedRange(
    UINT8* bitmap,
    EFI_PHYSICAL_ADDRESS pa,
    UINTN bytes
)
{
    UINT64 first = pa >> 12;
    UINT64 pages = (bytes + PAGE_SIZE - 1) >> 12;

    for (UINT64 i = 0; i < pages; ++i)
    {
        BITMAP_SET(bitmap, first + i);
    }
}

EFI_STATUS CreatePageBitmap(
    IN EFI_HANDLE imageHandle,
    IN EFI_MEMORY_DESCRIPTOR* memMap,
    IN UINTN mapSize,
    IN UINTN descSize,
    IN EFI_PHYSICAL_ADDRESS kernelBase,
    IN UINTN kernelSize,
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
    OUT VOID** bitmapBase,
    OUT UINT64* bitmapBytes
)
{
    EFI_STATUS status;
    UINT8* bitmap = NULL;

    if (memMap == NULL || bitmapBase == NULL || bitmapBytes == NULL || gop == NULL)
        return EFI_INVALID_PARAMETER;

    UINT64 highest = 0;
    for (EFI_MEMORY_DESCRIPTOR* d = memMap; (UINT8*)d < (UINT8*)memMap + mapSize; d = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)d + descSize))
    {
        if (d->Type != EfiConventionalMemory) continue;
        UINT64 end = d->PhysicalStart + (d->NumberOfPages << 12);
        if (end > highest) highest = end;
    }

    UINT64 maxPages = (highest + PAGE_SIZE - 1) >> 12;
    UINT64 bytes = (maxPages + 7) >> 3;

    bitmap = AllocatePool(bytes);
    if (bitmap == NULL) 
    {
        return EFI_OUT_OF_RESOURCES; 
    }

    SetMem(bitmap, bytes, 0xFF);

    for (EFI_MEMORY_DESCRIPTOR* d = memMap; (UINT8*)d < (UINT8*)memMap + mapSize; d = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)d + descSize))
    {
        if (d->Type != EfiConventionalMemory) continue;

        UINT64 first = d->PhysicalStart >> 12;
        for (UINT64 i = 0; i < d->NumberOfPages; ++i)
        {
            BITMAP_CLR(bitmap, first + i);
        }
    }

    EFI_LOADED_IMAGE_PROTOCOL *ld = NULL;
    status = gBS->HandleProtocol(imageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&ld);
    if (!EFI_ERROR(status))
    {
        SetUsedRange(bitmap, (UINT64)ld->ImageBase, ld->ImageSize);
    }

    SetUsedRange(bitmap, kernelBase, kernelSize);
    SetUsedRange(bitmap, gop->Mode->FrameBufferBase, gop->Mode->FrameBufferSize);
    SetUsedRange(bitmap, (UINT64)(UINTN)bitmap, bytes);
    BITMAP_SET(bitmap, 0);
    
    *bitmapBase = bitmap;
    *bitmapBytes = bytes;

    return EFI_SUCCESS;
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

    LOADER_DISPLAY_MODE mode = { .index = 0, .width = 0, .height = 0, .pixelFormat = PixelRedGreenBlueReserved8BitPerColor, .pixelsPerScanLine = 0 };
    VOID* elfBuffer = NULL;
    UINTN elfSize = 0;
    EFI_PHYSICAL_ADDRESS kernelBase = 0;
    UINTN kernelBaseSize = 0;
    VOID* kernelEntry = NULL;
    EFI_MEMORY_DESCRIPTOR* memMap = NULL;
    UINTN mapSize = 0;
    UINTN mapKey = 0;
    UINTN descSize = 0;
    UINT32 descVersion = 0;
    UINT64 frameBufferBase = 0;
    UINT64 frameBufferSize = 0;
    UINT8 bytePerPixel = 4;
    UINT8 isBGR = 0;
    UINT64 rootSystemDescriptionPointerAddress = 0;
    CHAR8* commandLine = NULL;
    LOADER_BOOT_INFO info = { 0 };

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

    status = LoadKernelElf(elfBuffer, elfSize, &kernelBase, &kernelBaseSize, &kernelEntry);
    FreePool(elfBuffer);
    elfBuffer = NULL;
    if (EFI_ERROR(status))
    {
        Print(L"[ERROR] LoadKernelElf : %r\n", status);
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

    status = GetMemoryMap(&memMap, &mapSize, &mapKey, &descSize, &descVersion);
    if (EFI_ERROR(status))
    {
        Print(L"[ERROR] GetMemoryMap : %r\n", status);
        Halt();
    }

    VOID* pageBitmap = NULL;
    UINT64 pageBitmapBytes = 0;

    status = CreatePageBitmap(imageHandle, memMap, mapSize, descSize, kernelBase, kernelBaseSize, gop, &pageBitmap, &pageBitmapBytes);
    if (EFI_ERROR(status))
    {
        Print(L"[ERROR] CreatePageBitmap : %r\n", status);
        Halt();
    }

    FreePool(memMap);
    memMap = NULL;
    mapSize = 0;
    mapKey = 0;
    descSize = 0;
    descVersion = 0;

    status = GetMemoryMap(&memMap, &mapSize, &mapKey, &descSize, &descVersion);
    if (EFI_ERROR(status))
    {
        Print(L"[ERROR] GetMemoryMap : %r\n", status);
        Halt();
    }

    status = gBS->ExitBootServices(imageHandle, mapKey);
    if (EFI_ERROR(status))
    {
        Print(L"[ERROR] ExitBootServices : %r\n", status);
        Halt();
    }

    if (mode.pixelFormat == PixelBlueGreenRedReserved8BitPerColor) isBGR = 1;

    info.memoryMap = memMap;
    info.memoryMapSize = mapSize;
    info.mapDescriptorSize = descSize;
    info.mapDescriptorVersion = descVersion;
    info.pageBitmapBase = (unsigned long long)(UINTN)pageBitmap;
    info.pageBitmapSize = (unsigned long long)pageBitmapBytes;
    info.kernelBase = (unsigned long long)kernelBase;
    info.kernelSize = (unsigned long long)kernelBaseSize;
    info.frameBufferBase = (unsigned long long)frameBufferBase;
    info.frameBufferSize = (unsigned long long)frameBufferSize;
    info.frameBufferWidth = mode.width;
    info.frameBufferHeight = mode.height;
    info.isBGR = isBGR;
    info.bytesPerPixel = bytePerPixel;
    info.pixelsPerScanLine = mode.pixelsPerScanLine;
    info.rootSystemDescriptionPointerAddress = (unsigned long long)rootSystemDescriptionPointerAddress;
    info.cmdLine = commandLine;

    ((void (*)(LOADER_BOOT_INFO*))kernelEntry)(&info);

    return EFI_SUCCESS;
}
