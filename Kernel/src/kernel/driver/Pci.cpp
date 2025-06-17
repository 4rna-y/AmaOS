#include "driver/Pci.h"

namespace pci
{
    static inline void outb(uint16_t p, uint8_t v)
    {
        __asm__ volatile(
            "outb %0,%1" 
            :
            : "a"(v), "Nd"(p)
        ); 
    }

    static inline uint8_t inb (uint16_t p)
    { 
        uint8_t  v; 
        __asm__ volatile(
            "inb %1,%0" 
            : "=a"(v) 
            : "Nd"(p)
        ); 
        return v; 
    }

    static inline void outw(uint16_t p, uint16_t v)
    { 
        __asm__ volatile(
            "outw %0,%1" 
            :
            : "a"(v), "Nd"(p)
        ); 
    }

    static inline uint16_t inw (uint16_t p)
    { 
        uint16_t v; 
        __asm__ volatile(
            "inw %1,%0" 
            : "=a"(v) 
            : "Nd"(p)
        ); 
        return v; 
    }

    static inline void outl(uint16_t p, uint32_t v)
    { 
        __asm__ volatile(
            "outl %0,%1" 
            :
            : "a"(v), "Nd"(p)
        ); 
    }

    static inline uint32_t inl (uint16_t p)
    { 
        uint32_t v; 
        __asm__ volatile(
            "inl %1,%0" 
            : "=a"(v) 
            : "Nd"(p)
        ); 
        return v; 
    }

    static inline void pci_addr(uint8_t bus, uint8_t dev, uint8_t func, uint8_t off)
    {
        uint32_t a = 0x80000000u | ((uint32_t)bus << 16) | ((uint32_t)dev << 11) | ((uint32_t)func << 8) | (off & 0xFC);
        outl(0xCF8, a);
    }

    static inline uint32_t read32(uint8_t b, uint8_t d, uint8_t f, uint8_t off)
    {
        pci_addr(b, d, f, off); 
        return inl(0xCFC);
    }
    static inline void write32(uint8_t b, uint8_t d, uint8_t f, uint8_t off, uint32_t v)
    {
        pci_addr(b, d, f, off); 
        outl(0xCFC, v);
    }
    static inline uint16_t read16(uint8_t b, uint8_t d, uint8_t f, uint8_t off)
    {
        pci_addr(b, d, f, off); 
        return inw(0xCFC | (off & 2));
    }
    static inline void write16(uint8_t b, uint8_t d, uint8_t f, uint16_t off, uint16_t v)
    {
        pci_addr(b, d, f, off); 
        outw(0xCFC | (off & 2), v);
    }

    volatile uint8_t* find_ahci_hba_base()
    {
        for (uint8_t bus = 0; bus < (unsigned char)0x100; ++bus)
        for (uint8_t dev = 0; dev < 32; ++dev)
        for (uint8_t func = 0; func < 8; ++func)
        {
            uint32_t id = read32(bus, dev, func, 0x00);
            if ((uint16_t)id == 0xFFFF) continue;              

            uint32_t classSig = read32(bus, dev, func, 0x08);
            uint8_t classCode = classSig >> 24 & 0xFF;
            uint8_t subClass = classSig >> 16 & 0xFF;
            uint8_t progIF = classSig >> 8 & 0xFF;

            if (classCode == 0x01 && subClass == 0x06 && progIF == 0x01)
            {
                uint32_t bar5_lo = read32(bus, dev, func, 0x24);
                uint32_t bar5_hi = read32(bus, dev, func, 0x28);
                uint64_t mmio = ((uint64_t)bar5_hi << 32) | (bar5_lo & 0xFFFFFFF0u);

                uint16_t cmd = read16(bus, dev, func, 0x04);
                cmd |= (1u << 1) | (1u << 2);
                write16(bus, dev, func, 0x04, cmd);

                return (volatile uint8_t*)(uintptr_t)mmio;
            }
        }
        return nullptr;
    }
}