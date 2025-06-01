#pragma once 

#include <stdint.h>
#include "../BootInfo.h"

typedef struct
{
    uint32_t type;
    uint32_t pad;
    uint64_t physicalStart;
    uint64_t virtualStart;
    uint64_t numberOfPages;
    uint64_t attribute;
} EFI_MEMORY_DESCRIPTOR;


#ifdef __cplusplus
extern "C" 
{
#endif

    void mm_initialize(BootInfo* info);
    void* mm_alloc_page();
    void mm_free_page();
    uint64_t mm_get_free_pages();
    
#ifdef __cplusplus
}
#endif