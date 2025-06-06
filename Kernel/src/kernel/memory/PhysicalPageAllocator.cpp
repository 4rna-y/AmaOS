#include "memory/PhysicalPageAllocator.h"

#define PAGE_SIZE 4096ULL

static uint8_t* g_bitmap = nullptr;
static uint64_t g_bitmap_bytes = 0;
static uint64_t g_max_pages = 0;
static uint64_t g_free_pages = 0;

static inline void set_used(uint64_t i)
{
    g_bitmap[i >> 3] |= (1u << (i & 7));
}

static inline void set_free(uint64_t i)
{
    g_bitmap[i >> 3] &= ~(1u << (i & 7));
}

static inline int is_free(uint64_t i)
{
    return (g_bitmap[i >> 3] & (1u << (i & 7))) == 0;
}

KERNEL_STATUS k_ppa_init(const LOADER_BOOT_INFO* info)
{
    if (!info || !info->pageBitmapBase || !info->pageBitmapSize) return KERNEL_FAILURE;
    g_bitmap = (uint8_t*)(uintptr_t)info->pageBitmapBase;
    g_bitmap_bytes = info->pageBitmapSize;
    g_max_pages = g_bitmap_bytes * 8;

    for (uint64_t b = 0; b < g_bitmap_bytes; ++b) {
        g_bitmap[b] = 0;
    }

    set_used(0);

    g_free_pages = 0;
    for (uint64_t i = 0; i < g_max_pages; ++i)
    {
        if (is_free(i)) ++g_free_pages;
    }

    return KERNEL_SUCCESS;
}

void* k_ppa_alloc()
{
    if (!g_bitmap) return nullptr;

    for (uint64_t i = 0; i < g_max_pages; ++i)
    {
        if (is_free(i))
        {
            set_used(i);
            --g_free_pages;

            return (void*)(uintptr_t)(i * PAGE_SIZE);
        }
    }

    return nullptr;
}

KERNEL_STATUS k_ppa_free(void* addr)
{
    if (!addr || ((uintptr_t)addr & (PAGE_SIZE - 1))) return KERNEL_FAILURE;
    uint64_t i = (uintptr_t)addr / PAGE_SIZE;
    if (i >= g_max_pages || is_free(i)) return KERNEL_FAILURE;

    set_free(i);
    ++g_free_pages;

    return KERNEL_SUCCESS;
}

uint64_t k_ppa_free_page_count()
{
    return g_free_pages;
}

