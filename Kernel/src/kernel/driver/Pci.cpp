#include "driver/Pci.h"

static inline void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static inline void outw(uint16_t port, uint16_t value)
{
    __asm__ volatile ("outw %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint16_t inw(uint16_t port)
{
    uint16_t value;
    __asm__ volatile ("inw %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static inline void outl(uint16_t port, uint32_t value)
{
    __asm__ volatile ("outl %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint32_t inl(uint16_t port)
{
    uint32_t value;
    __asm__ volatile ("inl %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static inline void pci_config_address(uint8_t bus, uint8_t dev, uint8_t func, uint8_t offset)
{
    uint32_t address = (1u << 31) | ((uint32_t)bus << 16) | ((uint32_t)dev << 11) | ((uint32_t)func << 8) | (offset & 0xFC);
    outl(0xCF8, address);
}

uint32_t k_pci_read32(
    uint8_t bus, 
    uint8_t dev, 
    uint8_t func, 
    uint8_t offset
)
{
    pci_config_address(bus, dev, func, offset);
    return inl(0xCFC);
}

void k_pci_write32(
    uint8_t bus, 
    uint8_t dev, 
    uint8_t func, 
    uint8_t offset, 
    uint32_t value
)
{
    pci_config_address(bus, dev, func, offset);
    outl(0xCFC, value);
}

volatile uint8_t* k_find_ahci_hba_base()
{
    for (uint8_t bus = 0; bus < 256; ++bus)
    {
        for (uint8_t dev = 0; dev < 32; ++dev)
        {
            for (uint8_t func = 0; func < 8; ++func)
            {
                uint32_t venDid = k_pci_read32(bus, dev, func, 0x00);
                uint16_t venderId = (uint16_t)(venDid & 0xFFFF);

                if (venderId == 0xFFFF) continue;

                uint32_t classSig = k_pci_read32(bus, dev, func, 0x08);
                uint8_t classCode = (uint8_t)((classSig >> 24) & 0xFF);
                uint8_t subClass = (uint8_t)((classSig >> 16) & 0xFF);
                uint8_t progIF = (uint8_t)((classSig >> 8) & 0xFF);

                if (classCode == 0x01 && subClass == 0x06 && progIF == 0x01)
                {
                    uint32_t bar5Low = k_pci_read32(bus, dev, func, 0x24);
                    uint32_t bar5High = k_pci_read32(bus, dev, func,0x28);
                    uint64_t phys = ((uint64_t)bar5High << 32) | ((uint64_t)bar5Low & 0xFFFFFFFFULL);
                    
                    phys &= ~0xFULL;

                    return (volatile uint8_t*)(uintptr_t)phys;
                }
            }
        }
    }   

    return nullptr;
}