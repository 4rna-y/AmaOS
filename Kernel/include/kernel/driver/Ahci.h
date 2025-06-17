#pragma once 

#include <stdint.h>

#include "../KernelStatus.h"

namespace ahci
{
    struct PortRegs {
        uint32_t is;     // Interrupt Status
        uint32_t serr;   // Error Status
        uint32_t tfd;    // Task-File Data
        uint32_t ci;     // Command Issue
        uint32_t cmd;    // Command & Status
        uint32_t ssts;   // SATA Status
    };

    KERNEL_STATUS init();
    KERNEL_STATUS read_disk_sectors(uint64_t lba, uint32_t count, void* buffer);
    KERNEL_STATUS write_disk_sectors(uint64_t lba, uint32_t count, const void* buffer);
    PortRegs dump_port_regs();
}