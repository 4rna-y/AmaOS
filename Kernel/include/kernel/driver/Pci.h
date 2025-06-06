#pragma once

#include <stdint.h>

uint32_t k_pci_read32(
    uint8_t bus, 
    uint8_t dev, 
    uint8_t func, 
    uint8_t offset
);

void k_pci_write32(
    uint8_t bus, 
    uint8_t dev, 
    uint8_t func, 
    uint8_t offset, 
    uint32_t value
);

volatile uint8_t* k_find_ahci_hba_base();