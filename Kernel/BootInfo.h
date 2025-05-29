#ifndef BOOTINFO_H
#define BOOTINFO_H

#include <stdint.h>

typedef struct 
{
    uint64_t memoryMapSize;
    void* memoryMap;
    uint64_t mapDescriptorSize;
    uint32_t mapDescriptorVersion;
    uint64_t frameBufferBase;
    uint64_t frameBufferSize;
    uint32_t fbWidth;
    uint32_t fbHeight;
    uint64_t rsdpAddress;
    char* cmdLine;
} BootInfo;

#endif // END BOOTINFO_H