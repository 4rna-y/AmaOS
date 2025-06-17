#pragma once
#include "KernelStatus.h"
#include "Types.h"

#include "../../LoaderBootInfo.h"

namespace ppa
{
    KERNEL_STATUS init(const LOADER_BOOT_INFO* info);
    void* alloc();
    KERNEL_STATUS free(void* addr);
    uint64_t free_page_count();
}
