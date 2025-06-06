#pragma once 

#include <stdint.h>

#include "../KernelStatus.h"

KERNEL_STATUS k_ahci_init();
KERNEL_STATUS k_ahci_read_disk_sectors(uint64_t lba, uint32_t count, void* buffer);
KERNEL_STATUS k_ahci_write_disk_sectors(uint64_t lba, uint32_t count, const void* buffer);