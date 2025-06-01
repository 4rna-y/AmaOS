#include "MemoryManager.h"

extern uint64_t _kernel_start;
extern uint64_t _kernel_end;

#define PAGE_SIZE 4096ULL
#define EFI_CONVENTIONAL_MEMORY 7U

static uint8_t* bitmap = nullptr;
static uint64_t bitmapBytes = 0;
static uint64_t totalPages = 0;
static uint64_t freePages = 0;

static inline void set_bit(uint64_t i)
{
    bitmap[i >> 3] |= (1u << (i & 7));
}

static inline void clear_bit(uint64_t i)
{
    bitmap[i >> 3] &= ~(1u << (i & 7));
}

static inline bool check_bit(uint64_t i)
{
    return bitmap[i >> 3] & (1u << (i & 7));
}