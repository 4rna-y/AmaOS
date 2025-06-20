#include "cpu/Idt.h"
#include "cpu/Exception.h"
#include "memory/MemoryOperator.h"
#include "debug/Log.h"

alignas(16) static IDT_ENTRY g_idt[256];
IDT_POINTER g_idt_ptr;

void set_gate(u8 vec, uptr handlerAddr, u8 ist)
{
    IDT_ENTRY& e = g_idt[vec];
    
    e.offsetLow = (u16)(handlerAddr & 0xFFFF);
    e.selector = 0x08;
    e.ist = ist & 0x7;
    e.zero = 0;
    e.typeAttr = 0x8E;
    e.offsetMid = (u16)((handlerAddr >> 16) & 0xFFFF);
    e.offsetHigh = (u32)(handlerAddr >> 32);
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

        memop::zero_mem(g_idt, sizeof(g_idt));

        for (u8 i = 0; i < 32; i++)
        {
            if (i == 14) continue;
            uptr hAddr = hasErr[i] ? reinterpret_cast<uptr>(dummy_err) : reinterpret_cast<uptr>(dummy_noerr);
            set_gate(i, hAddr, 0);
        }

        set_gate( 8, reinterpret_cast<uptr>(dummy_err) , 1); 
        set_gate(14, reinterpret_cast<uptr>(page_fault), 2);

        g_idt_ptr.limit = sizeof(g_idt) - 1;
        g_idt_ptr.base = reinterpret_cast<u64>(g_idt);

        if (reinterpret_cast<uptr>(page_fault) == 0)
        {
            debug::print("null");
        }
        
        
        debug::print(reinterpret_cast<uptr>(page_fault), true);
        debug::print(g_idt[14].offsetLow     , true);
        debug::print(g_idt[14].offsetMid     , true);
        debug::print(g_idt[14].offsetHigh    , true);
        debug::print(g_idt[14].ist           , true);
        debug::print(g_idt[14].typeAttr      , true);
    }

    const IDT_POINTER* ptr() { return &g_idt_ptr; }    
}

