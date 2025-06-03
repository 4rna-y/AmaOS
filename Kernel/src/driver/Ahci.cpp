#include "driver/Ahci.h"

#include <stdint.h>
#include "driver/Pci.h"
#include "memory/PhysicalPageAllocator.h"

#define HBA_CAP       0x00
#define HBA_GHC       0x04
#define HBA_IS        0x08
#define HBA_PI        0x0C
#define HBA_VS        0x10
#define HBA_CCC_CTL   0x14
#define HBA_CCC_PORTS 0x18

#define PORT_CLB      0x00
#define PORT_FB       0x08
#define PORT_IS       0x10
#define PORT_CMD      0x18
#define PORT_TFD      0x20
#define PORT_SIG      0x24
#define PORT_SSTS     0x28
#define PORT_SCTL     0x2C
#define PORT_SERR     0x30
#define PORT_SACT     0x34
#define PORT_CI       0x38

typedef struct 
{
    uint16_t cfl : 5;
    uint16_t a : 1;
    uint16_t w : 1;
    uint16_t p : 1;
    uint16_t r : 1;
    uint16_t b : 1;
    uint16_t c : 1;
    uint16_t reserved0 : 1;
    uint16_t pmp : 4;
    uint16_t prdtl;      

    volatile uint32_t prdbc;

    uint32_t ctba; 
    uint32_t ctbau;

    uint32_t reserved1[4];
} HBA_CMD_HEADER;

typedef struct 
{
    uint32_t dba;    
    uint32_t dbau;   
    uint32_t reserved0;
    uint32_t dbc_ioc;
} HBA_PRDT_ENTRY;

typedef struct 
{
    uint8_t  cfis[64];
    uint8_t  reserved[192];
    HBA_PRDT_ENTRY prdt_entry[1];
} HBA_CMD_TBL;

typedef struct 
{
    uint8_t fis_type;  
    uint8_t pmport : 4;
    uint8_t rsv0 : 3;
    uint8_t c : 1;
    uint8_t command;   
    uint8_t featurel;  
    uint8_t lba0;      
    uint8_t lba1;      
    uint8_t lba2;      
    uint8_t device;    
    uint8_t lba3;      
    uint8_t lba4;      
    uint8_t lba5;      
    uint8_t featureh;  
    uint8_t countl;    
    uint8_t counth;    
    uint8_t icc;       
    uint8_t control;   
    uint8_t rsv1[4];   
} FIS_REG_H2D;

static volatile uint8_t* g_hba = nullptr;
static int g_port_index = -1;
static uint64_t g_port_clb_phys = 0;
static uint64_t g_port_fb_phys = 0;

static inline uint32_t hba_read32(uint32_t offset)
{
    return *(volatile uint32_t*)(g_hba + offset);
}

static inline void hba_write32(uint32_t offset, uint32_t value)
{
    *(volatile uint32_t*)(g_hba + offset) = value;
}

static inline void hba_write32_mask(uint32_t offset, uint32_t mask, uint32_t value)
{
    uint32_t original = *(volatile uint32_t*)(g_hba + offset);
    original &= ~mask;
    original |= (value & mask);
    *(volatile uint32_t*)(g_hba + offset) = original;
}

static inline uint32_t port_read32(int port, uint32_t offset)
{
    return *(volatile uint32_t*)(g_hba + 0x100 + (port * 0x80) + offset);
}

static inline void port_write32(int port, uint32_t offset, uint32_t value) 
{
    *(volatile uint32_t*)(g_hba + 0x100 + (port * 0x80) + offset) = value;
}

static inline void port_write32_mask(int port, uint32_t offset, uint32_t mask, uint32_t value) 
{
    uint8_t* base = (uint8_t*)g_hba + 0x100 + (port * 0x80) + offset;
    uint32_t original = *(volatile uint32_t*)base;
    original &= ~mask;
    original |= (value & mask);
    *(volatile uint32_t*)base = original;
}

KERNEL_STATUS k_ahci_init()
{
    volatile uint8_t* base = k_find_ahci_hba_base();
    if (base == nullptr) return KERNEL_FAILURE;
    g_hba = base;

    uint32_t ghc = hba_read32(HBA_GHC);
    ghc |= (1u << 31);
    hba_write32(HBA_GHC, ghc);

    uint32_t pi = hba_read32(HBA_PI);
    if (pi == 0) return KERNEL_FAILURE;

    int selected = -1;
    for (int port = 0; port < 32; ++port)
    {
        if (pi & (1u << port))
        {
            uint32_t ssts = port_read32(port, PORT_SSTS);
            uint8_t det = (uint8_t)(ssts & 0xF);
            uint8_t ipm = (uint8_t)((ssts >> 8) & 0xF);
            if (det == 0x3 && ipm == 0x1)
            {
                selected = port;
                break;
            }
        }
    }

    if (selected < 0) return KERNEL_FAILURE;
    g_port_index = selected;

    g_port_clb_phys = (uint64_t)k_ppa_alloc();
    if (g_port_clb_phys == 0) return KERNEL_FAILURE;

    g_port_fb_phys = (uint64_t)k_ppa_alloc();
    if (g_port_fb_phys == 0)
    {
        k_ppa_free((void*)(uintptr_t)g_port_clb_phys);
        g_port_clb_phys = 0;
        return KERNEL_FAILURE;
    }

    port_write32(g_port_index, PORT_CLB + 0, (uint32_t)(g_port_clb_phys & 0xFFFFFFFF));
    port_write32(g_port_index, PORT_CLB + 4, (uint32_t)(g_port_clb_phys >> 32));
    port_write32(g_port_index, PORT_FB + 0, (uint32_t)(g_port_fb_phys & 0xFFFFFFFF));
    port_write32(g_port_index, PORT_FB + 4, (uint32_t)(g_port_fb_phys >> 32));

    port_write32_mask(g_port_index, PORT_CMD, (1u << 15) | (1u << 0), 0);
    port_write32_mask(g_port_index, PORT_CMD, (1u << 14), (1u << 14));
    port_write32_mask(g_port_index, PORT_CMD, (1u << 0), (1u << 0));

    return KERNEL_SUCCESS;
}

KERNEL_STATUS k_ahci_read_disk_sectors(uint64_t lba, uint32_t count, void* buffer)
{
    if (g_hba == nullptr || g_port_index < 0) return KERNEL_FAILURE;
    if (count == 0 || buffer == nullptr) return KERNEL_FAILURE;
    if ((uint64_t)count * 512 > (uint64_t)(4ull * 1024 * 1024)) return KERNEL_FAILURE;

    HBA_CMD_HEADER* cmdHeader = (HBA_CMD_HEADER*)(uintptr_t)g_port_clb_phys;
    for (long i = 0; i < sizeof(HBA_CMD_HEADER) / 4; ++i)
    {
        ((uint32_t*)cmdHeader)[i] = 0;
    }

    cmdHeader->cfl = sizeof(FIS_REG_H2D) / 4;
    cmdHeader->prdtl = 1;
    cmdHeader->w = 0;
    cmdHeader->p = 1;
    cmdHeader->a = 0;
    cmdHeader->c = 0;
    cmdHeader->b = 0;
    cmdHeader->pmp = 0;

    HBA_CMD_TBL* cmdTable = (HBA_CMD_TBL*)(uintptr_t)g_port_fb_phys;
    for (int i = 0; i < 256 / 4; ++i)
    {
        ((uint32_t*)cmdTable)[i] = 0;
    }

    HBA_PRDT_ENTRY* prdt = &cmdTable->prdt_entry[0];
    uint64_t bufPhys = (uint64_t)buffer;
    prdt->dba = (uint32_t)(bufPhys & 0xFFFFFFFF);
    prdt->dbau = (uint32_t)(bufPhys >> 32);
    prdt->dbc_ioc = ((count * 512) - 1) & 0x3FFFFF;
    prdt->dbc_ioc |= (1u << 31);

    FIS_REG_H2D* fis = (FIS_REG_H2D*)&cmdTable->cfis[0];
    for (long i = 0; i < sizeof(FIS_REG_H2D) / 4; ++i)
    {
        ((uint32_t*)fis)[i] = 0;
    }

    fis->fis_type = 0x27;
    fis->pmport = (1 << 7);
    fis->command = 0x25;
    fis->lba0 = (uint8_t)(lba & 0xFF);
    fis->lba1 = (uint8_t)((lba >> 8) & 0xFF);
    fis->lba2 = (uint8_t)((lba >> 16) & 0xFF);
    fis->lba3 = (uint8_t)((lba >> 24) & 0xFF);
    fis->lba4 = (uint8_t)((lba >> 32) & 0xFF);
    fis->lba5 = (uint8_t)((lba >> 40) & 0xFF);
    fis->device = (1 << 6);
    fis->countl = (uint8_t)(count & 0xFF);
    fis->counth = (uint8_t)((count >> 8) & 0xFF);
    
    cmdHeader->ctba = (uint32_t)(g_port_fb_phys & 0xFFFFFFFF);
    cmdHeader->ctbau = (uint32_t)(g_port_fb_phys >> 32);

    port_write32_mask(g_port_index, PORT_CI, (1u << 0), (1u << 0));

    for (volatile int t = 0; t < 100000000; ++t) 
    {
        uint32_t tfd = port_read32(g_port_index, PORT_TFD);
        if ((tfd & (1u << 7)) == 0) 
        {
            if ((port_read32(g_port_index, PORT_CI) & (1u << 0)) == 0) 
            {
                return KERNEL_SUCCESS;
            }
        }
    }

    return KERNEL_FAILURE;
}

KERNEL_STATUS k_ahci_write_disk_sectors(uint64_t lba, uint32_t count, const void* buffer) 
{
    if (g_hba == nullptr || g_port_index < 0) return KERNEL_FAILURE;
    if (count == 0 || buffer == nullptr) return KERNEL_FAILURE;
    if ((uint64_t)count * 512 > (uint64_t)(4ull * 1024 * 1024)) return KERNEL_FAILURE;
    
    volatile uint32_t* port = (volatile uint32_t*)(g_hba + 0x100 + (g_port_index * 0x80));

    HBA_CMD_HEADER* cmdHeader = (HBA_CMD_HEADER*)(uintptr_t)g_port_clb_phys;
    for (long i = 0; i < sizeof(HBA_CMD_HEADER) / 4; ++i) 
    {
        ((uint32_t*)cmdHeader)[i] = 0;
    }
    cmdHeader->cfl = sizeof(FIS_REG_H2D) / 4;
    cmdHeader->prdtl = 1;
    cmdHeader->w = 1;
    cmdHeader->p = 1;
    cmdHeader->a = 0;
    cmdHeader->c = 0;
    cmdHeader->b = 0;
    cmdHeader->pmp = 0;

    HBA_CMD_TBL* cmdTable = (HBA_CMD_TBL*)(uintptr_t)g_port_fb_phys;
    for (int i = 0; i < 256 / 4; ++i) 
    {
        ((uint32_t*)cmdTable)[i] = 0;
    }

    HBA_PRDT_ENTRY* prdt = &cmdTable->prdt_entry[0];
    uint64_t buf_phys = (uint64_t)buffer;
    prdt->dba = (uint32_t)(buf_phys & 0xFFFFFFFF);
    prdt->dbau = (uint32_t)(buf_phys >> 32);
    prdt->dbc_ioc = ((count * 512) - 1) & 0x3FFFFF;
    prdt->dbc_ioc |= (1u << 31);

    FIS_REG_H2D* fis = (FIS_REG_H2D*)&cmdTable->cfis[0];
    for (long i = 0; i < sizeof(FIS_REG_H2D) / 4; ++i) 
    {
        ((uint32_t*)fis)[i] = 0;
    }
    fis->fis_type = 0x27;
    fis->pmport = (1 << 7);
    fis->command = 0x35;      
    fis->lba0 = (uint8_t)(lba & 0xFF);
    fis->lba1 = (uint8_t)((lba >> 8) & 0xFF);
    fis->lba2 = (uint8_t)((lba >> 16) & 0xFF);
    fis->device = 1 << 6;
    fis->lba3 = (uint8_t)((lba >> 24) & 0xFF);
    fis->lba4 = (uint8_t)((lba >> 32) & 0xFF);
    fis->lba5 = (uint8_t)((lba >> 40) & 0xFF);
    fis->countl = (uint8_t)(count & 0xFF);
    fis->counth = (uint8_t)((count >> 8) & 0xFF);

    cmdHeader->ctba  = (uint32_t)(g_port_fb_phys & 0xFFFFFFFF);
    cmdHeader->ctbau = (uint32_t)(g_port_fb_phys >> 32);

    port_write32_mask(g_port_index, PORT_CI, (1u << 0), (1u << 0));

    for (volatile int t = 0; t < 100000000; ++t) 
    {
        uint32_t tfd = port_read32(g_port_index, PORT_TFD);
        if ((tfd & (1u << 7)) == 0) 
        {
            if ((port_read32(g_port_index, PORT_CI) & (1u << 0)) == 0) {
                return KERNEL_SUCCESS;
            }
        }
    }
    return KERNEL_FAILURE;
}