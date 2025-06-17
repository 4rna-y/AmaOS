#include "cpu/Cpu.h"
#include "cpu/Gdt.h"
#include "cpu/Idt.h"
#include "cpu/Tss.h"
#include "debug/Log.h"

extern "C" void cpu_lgdt(const GDT_POINTER*);
extern "C" void cpu_lidt(const IDT_POINTER*);
extern "C" void cpu_load_tss_m16(u16);
extern "C" void cpu_far_switch();

namespace cpu
{

    static inline void outb(uint16_t port, uint8_t val)
    {
        __asm__ volatile ("outb %b0, %w1" :: "a"(val), "Nd"(port));
    }

    inline void mask_pic()
    {
        outb(0x21, 0xFF);
        outb(0xA1, 0xFF);
    }

    static inline void halt()
    {
        for (;;) { __asm__ volatile("hlt"); }
    }

    void init_bsp()
    {
        __asm__ volatile("cli");
        gdt::build_table();
        tss::init();
        cpu_lgdt(gdt::ptr());
        idt::build_exceptions();
        cpu_lidt(idt::ptr());
        cpu_load_tss_m16(0x18);
        cpu_far_switch();      
        mask_pic();
        __asm__ volatile("sti");
    }

    void init_ap()
    {
        __asm__ volatile("cli");
        gdt::build_table();
        cpu_lgdt(gdt::ptr());
        cpu_far_switch();
        cpu_load_tss_m16(3 * 8);
        cpu_lidt(idt::ptr());
        __asm__ volatile("sti");
    }
}