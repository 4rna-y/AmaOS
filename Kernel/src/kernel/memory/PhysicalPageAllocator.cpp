#include "memory/PhysicalPageAllocator.h"

#define PAGE_SIZE 4096ULL

namespace ppa 
{
    static u8* g_bitmap = nullptr;
    static u64 g_bitmap_bytes = 0;
    static u64 g_max_pages = 0;
    static u64 g_free_pages = 0;

    static inline void set_used(u64 i)
    {
        g_bitmap[i >> 3] |= (1u << (i & 7));
    }

    static inline void set_free(u64 i)
    {
        g_bitmap[i >> 3] &= ~(1u << (i & 7));
    }

    static inline int is_free(u64 i)
    {
        return (g_bitmap[i >> 3] & (1u << (i & 7))) == 0;
    }

    KERNEL_STATUS init(const LOADER_BOOT_INFO* info)
    {
        if (!info || !info->pageBitmapBase || !info->pageBitmapSize) return KERNEL_FAILURE;

        g_bitmap = reinterpret_cast<u8*>(static_cast<uintptr_t>(info->pageBitmapBase));
        g_bitmap_bytes = info->pageBitmapSize;
        g_max_pages = g_bitmap_bytes * 8ULL;

        set_used(0);

        g_free_pages = 0;
        for (u64 byte = 0; byte < g_bitmap_bytes; ++byte)
        {
            u8 v = g_bitmap[byte];
            if (v == 0xFF) continue;
            for (u8 bit = 0; bit < 8; ++bit)
            {
                u64 page = byte * 8 + bit;
                if (page >= g_max_pages) break;
                if ((v & (1u << bit)) == 0) ++g_free_pages;
            }
        }

        return KERNEL_SUCCESS;
    }

    void* alloc()
    {
        if (!g_bitmap) return nullptr;

        for (u64 i = 0; i < g_max_pages; ++i)
        {
            if (is_free(i))
            {
                set_used(i);
                --g_free_pages;

                return reinterpret_cast<void*>(static_cast<uptr>(i * PAGE_SIZE));
            }
        }

        return nullptr;
    }

    KERNEL_STATUS free(void* addr)
    {
        if (!addr || (reinterpret_cast<uintptr_t>(addr) & (PAGE_SIZE - 1))) 
            return KERNEL_FAILURE;

        u64 i = reinterpret_cast<uintptr_t>(addr) / PAGE_SIZE;
        if (i >= g_max_pages || is_free(i)) 
            return KERNEL_FAILURE;

        set_free(i);
        ++g_free_pages;

        return KERNEL_SUCCESS;
    }

    uint64_t free_page_count()
    {
        return g_free_pages;
    }
}