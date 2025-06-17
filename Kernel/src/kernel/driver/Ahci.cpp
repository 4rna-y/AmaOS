#include "driver/Ahci.h"

#include <stdint.h>
#include <stddef.h>
#include "driver/Pci.h"
#include "memory/PhysicalPageAllocator.h"

#define HBA_GHC   0x04
#define HBA_PI    0x0C

#define PORT_CLB  0x00
#define PORT_FB   0x08
#define PORT_IS   0x10
#define PORT_CMD  0x18
#define PORT_TFD  0x20
#define PORT_SSTS 0x28
#define PORT_SERR 0x30
#define PORT_CI   0x38

#define CMD_ST   (1u << 0)
#define CMD_FRE  (1u << 4)
#define CMD_FR   (1u << 14)
#define CMD_CR   (1u << 15)
#define TFD_BSY  (1u << 7)
#define TFD_DRQ  (1u << 3)
#define IS_ERR_MASK 0x400000FFu 

typedef struct 
{
    u16 cfl : 5;
    u16 a   : 1;
    u16 w   : 1;
    u16 p   : 1;
    u16 r   : 1;
    u16 b   : 1;
    u16 c   : 1;
    u16 rsv : 1;
    u16 pmp : 4;
    u16 prdtl;
    volatile u32 prdbc;
    u32 ctba;
    u32 ctbau;
    u32 rsv1[4];
} HBA_CMD_HEADER;

typedef struct 
{
    u32 dba;
    u32 dbau;
    u32 rsv0;
    u32 dbc_ioc;
} HBA_PRDT_ENTRY;

typedef struct 
{
    u8  cfis[64];
    u8  rsv[192];
    HBA_PRDT_ENTRY prdt_entry[1];
} HBA_CMD_TBL;

typedef struct 
{
    u8 fis_type;
    u8 pmport : 4;
    u8 rsv0   : 3;
    u8 c      : 1;
    u8 command;
    u8 featurel;
    u8 lba0;
    u8 lba1;
    u8 lba2;
    u8 device;
    u8 lba3;
    u8 lba4;
    u8 lba5;
    u8 featureh;
    u8 countl;
    u8 counth;
    u8 icc;
    u8 control;
    u8 rsv1[4];
} FIS_REG_H2D;

namespace ahci 
{
    static volatile u8* g_hba = nullptr;
    static int g_port        = -1;
    static u64 g_clb_phys    = 0; 
    static u64 g_fb_phys     = 0;
    static u64 g_ctba_phys   = 0;

    static inline u32 hba_rd32(u32 off) 
    {
        return *(volatile u32*)(g_hba + off);
    }

    static inline void hba_wr32(u32 off, u32 v) 
    {
        *(volatile u32*)(g_hba + off) = v;
    }

    static inline u32 prd32(int port, u32 off) 
    {
        return *(volatile u32*)(g_hba + 0x100 + port * 0x80 + off);
    }

    static inline void pw32(int port, u32 off, u32 v) 
    {
        *(volatile u32*)(g_hba + 0x100 + port * 0x80 + off) = v;
    }

    static inline void pw32_mask(int port, u32 off, u32 m, u32 v) 
    {
        volatile u32* p = (volatile u32*)(g_hba + 0x100 + port * 0x80 + off);
        u32 o = *p;  
        o = (o & ~m) | (v & m);  
        *p = o;
    }

    static inline void io_delay() 
    { 
        asm volatile(
            "" 
            :
            :
            : "memory"
        ); 
    }

    static bool wait_ready() 
    {
        for (u32 i = 0; i < 1000000; ++i) 
        {
            if ((prd32(g_port, PORT_TFD) & (TFD_BSY|TFD_DRQ)) == 0) return true;
            io_delay();
        }

        return false;
    }

    static bool wait_complete() 
    {
        for (u32 i = 0; i < 2000000; ++i) 
        {
            u32 is  = prd32(g_port, PORT_IS);
            u32 ci  = prd32(g_port, PORT_CI);

            if (is & IS_ERR_MASK) return false;
            if ((ci & 1u) == 0) return true;

            io_delay();
        }

        return false;
    }

    KERNEL_STATUS init()
    {
        g_hba = pci::find_ahci_hba_base();
        if (!g_hba) return KERNEL_FAILURE;

        hba_wr32(HBA_GHC, hba_rd32(HBA_GHC) | (1u<<31));

        u32 pi = hba_rd32(HBA_PI);
        for (int p = 0; p < 32; ++p)
        {
            if (pi & (1u << p)) 
            {
                u32 ssts = prd32(p, PORT_SSTS);
                if ((ssts & 0xF) == 3 && ((ssts >> 8) & 0xF) == 1) 
                { 
                    g_port=p; 
                    break; 
                }
            }
        }

        if (g_port < 0) return KERNEL_FAILURE;

        g_clb_phys  = (u64)ppa::alloc();
        g_fb_phys   = (u64)ppa::alloc();
        g_ctba_phys = (u64)ppa::alloc();
        if (!g_clb_phys || !g_fb_phys || !g_ctba_phys) 
            return KERNEL_FAILURE;

        pw32_mask(g_port, PORT_CMD, CMD_ST|CMD_FRE, 0);

        while (prd32(g_port, PORT_CMD) & (CMD_CR|CMD_FR)) io_delay();

        pw32(g_port, PORT_CLB,  (u32)g_clb_phys);
        pw32(g_port, PORT_CLB + 4,(u32)(g_clb_phys >> 32));
        pw32(g_port, PORT_FB,   (u32)g_fb_phys);
        pw32(g_port, PORT_FB + 4, (u32)(g_fb_phys >> 32));

        pw32(g_port, PORT_IS, 0xFFFFFFFF);
        pw32(g_port, PORT_SERR, 0xFFFFFFFF);

        pw32_mask(g_port, PORT_CMD, CMD_FRE, CMD_FRE);
        pw32_mask(g_port, PORT_CMD, CMD_ST,  CMD_ST);

        return wait_ready() ? KERNEL_SUCCESS : KERNEL_FAILURE;
    }

    static KERNEL_STATUS issue(bool write, u64 lba, u32 cnt, void* buf)
    {
        if (!wait_ready()) return KERNEL_FAILURE;

        HBA_CMD_HEADER* hdr = (HBA_CMD_HEADER*)(uptr)g_clb_phys;
        for (size_t i = 0; i < sizeof(HBA_CMD_HEADER) / 4; ++i) 
        {
            ((u32*)hdr)[i] = 0;
        }

        hdr->cfl = sizeof(FIS_REG_H2D) / 4;
        hdr->w   = write;
        hdr->p   = 0;
        hdr->prdtl = 1;
        hdr->ctba  = (u32)g_ctba_phys;
        hdr->ctbau = (u32)(g_ctba_phys>>32);
        hdr->prdbc = 0;

        HBA_CMD_TBL* tbl = (HBA_CMD_TBL*)(uptr)g_ctba_phys;
        for (size_t i = 0; i < (sizeof(HBA_CMD_TBL) +3) / 4; ++i) 
        {
            ((u32*)tbl)[i] = 0;
        } 

        u64 phys = (u64)buf;
        tbl->prdt_entry[0].dba = (u32)phys;
        tbl->prdt_entry[0].dbau = (u32)(phys >> 32);
        tbl->prdt_entry[0].dbc_ioc = ((cnt * 512) - 1) | (1u << 31);

        FIS_REG_H2D* fis = (FIS_REG_H2D*)tbl->cfis;
        fis->fis_type = 0x27;
        fis->c        = 1;
        fis->command  = write ? 0x35 : 0x25;
        fis->device   = 1 << 6;
        fis->lba0 = (u8) lba;
        fis->lba1 = (u8)(lba >> 8);    
        fis->lba2 = (u8)(lba >> 16);
        fis->lba3 = (u8)(lba >> 24);   
        fis->lba4 = (u8)(lba >> 32);
        fis->lba5 = (u8)(lba >> 40);
        fis->countl = (u8) cnt;
        fis->counth = (u8)(cnt >> 8);

        while (prd32(g_port, PORT_CI) & 1u) 
            io_delay();
        pw32(g_port, PORT_IS, 0xFFFFFFFF);
        pw32_mask(g_port, PORT_CI, 1u, 1u);

        if (!wait_complete()) return KERNEL_FAILURE;
        pw32(g_port, PORT_IS, 0xFFFFFFFF);
        return KERNEL_SUCCESS;
    }

    KERNEL_STATUS read_disk_sectors(u64 lba, u32 cnt, void* buf)
    {
        return issue(false, lba, cnt, buf);
    }
    KERNEL_STATUS write_disk_sectors(u64 lba, u32 cnt, const void* buf)
    {
        return issue(true, lba, cnt, const_cast<void*>(buf));
    }

    PortRegs dump_port_regs()
    {
        PortRegs r{};
        r.is   = prd32(g_port, PORT_IS);
        r.serr = prd32(g_port, PORT_SERR);
        r.tfd  = prd32(g_port, PORT_TFD);
        r.ci   = prd32(g_port, PORT_CI);
        r.cmd  = prd32(g_port, PORT_CMD);
        r.ssts = prd32(g_port, PORT_SSTS);
        return r;
    }
}