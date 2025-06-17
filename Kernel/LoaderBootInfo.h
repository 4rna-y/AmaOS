#ifndef LOADER_BOOT_INFO_H
#define LOADER_BOOT_INFO_H

#include <stdint.h>

enum {
    LoaderReservedMemoryType,
    LoaderLoaderCode,
    LoaderLoaderData,
    LoaderBootServicesCode,
    LoaderBootServicesData,
    LoaderRuntimeServicesCode,
    LoaderRuntimeServicesData,
    LoaderConventionalMemory,
    LoaderUnusableMemory,
    LoaderACPIReclaimMemory,
    LoaderACPIMemoryNVS,
    LoaderMemoryMappedIO,
    LoaderMemoryMappedIOPortSpace,
    LoaderPalCode,
    LoaderPersistentMemory,
    LoaderUnacceptedMemoryType,
    LoaderMaxMemoryType
};


#define LOADER_CONVENTIONAL_MEMORY 7U

typedef struct _loader_memory_descriptor
{
    uint32_t type;
    uint64_t physicalStart;
    uint64_t virtualStart;
    uint64_t numberOfPages;
    uint64_t attribute; 
} LOADER_MEMORY_DESCRIPTOR;


typedef struct _loader_boot_info
{
    uint64_t memoryMapSize;
    void* memoryMap;
    uint64_t mapDescriptorSize;
    uint32_t mapDescriptorVersion;
    uint64_t pageBitmapBase;
    uint64_t pageBitmapSize;
    uint64_t kernelBase;
    uint64_t kernelSize;
    uint64_t frameBufferBase;
    uint64_t frameBufferSize;
    uint32_t frameBufferWidth;
    uint32_t frameBufferHeight;
    uint32_t pixelsPerScanLine;
    uint8_t bytesPerPixel;
    uint8_t isBGR;
    uint64_t rootSystemDescriptionPointerAddress;
    char* cmdLine;
}LOADER_BOOT_INFO;

#endif // END LOADER_BOOT_INFO_H