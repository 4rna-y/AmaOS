#include "driver/Ahci.h"

#include <stdint.h>
#include "driver/Pci.h"
#include "memory/PhysicalPageAllocator.h"

#define HBA_CAP 0x00
#define HBA_GHC 0x04
#define HBA_IS 0x08
#define HBA_PI 0x0C
#define HBA_VS 0x10
#define HBA_CCC_CTL 0x14
#define HBA_CCC_PORTS 0x18

#define PORT_CLB 0x00
#define PORT_FB 0x08
#define PORT_IS 0x10
#define PORT_CMD 0x18
#define PORT_TFD 0x20
#define PORT_SIG 0x24
#define PORT_SSTS 0x28
#define PORT_SCTL 0x2C
#define PORT_SERR 0x30
#define PORT_SACT 0x34
#define PORT_CI 0x38

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
    uint8_t cfis[64];
    uint8_t reserved[192];
    HBA_PRDT_ENTRY prdt_entry[1];
} HBA_CMD_TBL;


