#ifndef LOADER_BOOT_INFO_H
#define LOADER_BOOT_INFO_H

#include <stdint.h>

typedef struct 
{
    uint64_t memoryMapSize;
    void* memoryMap;
    uint64_t mapDescriptorSize;
    uint32_t mapDescriptorVersion;
    uint64_t pageBitmapBase;
    uint64_t pageBitmapSize;
    uint64_t frameBufferBase;
    uint64_t frameBufferSize;
    uint32_t frameBufferWidth;
    uint32_t frameBufferHeight;
    uint64_t rootSystemDescriptionPointerAddress;
    char* cmdLine;
}LOADER_BOOT_INFO;

#endif // END LOADER_BOOT_INFO_H