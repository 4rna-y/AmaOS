#pragma once

#include <stdint.h>

namespace pci
{
    volatile uint8_t* find_ahci_hba_base();
}