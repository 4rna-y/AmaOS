#pragma once 

#include <stdint.h>

#include "../KernelStatus.h"

KERNEL_STATUS ahci_init();
KERNEL_STATUS ahci_read_disk_sectors(uint64_t lba, uint32_t count, void* buffer);
KERNEL_STATUS ahci_write_disk_sectors(uint64_t lba, uint32_t count, const void* buffer);