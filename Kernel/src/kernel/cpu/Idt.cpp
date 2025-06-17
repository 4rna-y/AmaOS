#include "cpu/Idt.h"
#include "cpu/Exception.h"
#include "memory/MemoryOperator.h"

alignas(16) IDT_ENTRY g_idt[256];
IDT_POINTER g_idt_ptr;

void set_gate(u8 vec, void (*h)(), u8 ist)
{
    u64 addr = reinterpret_cast<u64>(h);
    IDT_ENTRY& e = g_idt[vec];
    
    e.offsetLow = addr & 0xFFFF;
    e.selector = 0x08;
    e.ist = ist & 0x7;
    e.zero = 0;
    e.typeAttr = 0x8E;
    e.offsetMid = (addr >> 16) & 0xFFFF;
    e.offsetHigh = addr >> 32;
    e.zero2 = 0;
}

namespace idt
{
    void build_exceptions()
    {
        const bool hasErr[32] = {
            0, 0, 0, 0, 0, 0, 0, 0, 
            1, 0, 1, 1, 1, 1, 1, 0,
            0, 1, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 1, 0 
        };

        for (u8 i = 0; i < 32; i++)
        {
            if (i == 14) continue;
            set_gate(i, hasErr[i] ? (void(*)())dummy_err : (void(*)())dummy_noerr, 0);
        }

        set_gate( 8, (void(*)())dummy_err , 1); 
        set_gate(14, (void(*)())page_fault, 2);

        g_idt_ptr.limit = sizeof(g_idt) - 1;
        g_idt_ptr.base = reinterpret_cast<u64>(g_idt);
    }

    const IDT_POINTER* ptr() { return &g_idt_ptr; }    
}

