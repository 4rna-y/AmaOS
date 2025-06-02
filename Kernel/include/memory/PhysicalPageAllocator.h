#pragma once
#include "KernelStatus.h"
#include <stdint.h>

#include "../../LoaderBootInfo.h"

#ifdef __cplusplus
extern "C" 
{
#endif

    KERNEL_STATUS k_ppa_init(const LOADER_BOOT_INFO* info);
    void* k_ppa_alloc();
    KERNEL_STATUS k_ppa_free(void* addr);
    uint64_t k_ppa_free_page_count();

#ifdef __cplusplus
}
#endif