#include "cpu/Exception.h"
#include "panic/PanicFrameWriter.h"
#include "debug/Log.h"

__attribute__((no_caller_saved_registers))
static inline void putc(char c)
{
    __asm__ volatile("outb %0, $0xE9" :: "a"(c));
}

static inline void fill_registers(PANIC_INFO& pinfo)
{
    __asm__ volatile("mov %%rax,%0":"=m"(pinfo.registers[0]));
    __asm__ volatile("mov %%rbx,%0":"=m"(pinfo.registers[1]));
    __asm__ volatile("mov %%rcx,%0":"=m"(pinfo.registers[2]));
    __asm__ volatile("mov %%rdx,%0":"=m"(pinfo.registers[3]));
    __asm__ volatile("mov %%rsi,%0":"=m"(pinfo.registers[4]));
    __asm__ volatile("mov %%rdi,%0":"=m"(pinfo.registers[5]));
    __asm__ volatile("mov %%rbp,%0":"=m"(pinfo.registers[6]));
    __asm__ volatile("mov %%r8,%0" :"=m"(pinfo.registers[8]));
    __asm__ volatile("mov %%r9,%0" :"=m"(pinfo.registers[9]));
    __asm__ volatile("mov %%r10,%0":"=m"(pinfo.registers[10]));
    __asm__ volatile("mov %%r11,%0":"=m"(pinfo.registers[11]));
    __asm__ volatile("mov %%r12,%0":"=m"(pinfo.registers[12]));
    __asm__ volatile("mov %%r13,%0":"=m"(pinfo.registers[13]));
    __asm__ volatile("mov %%r14,%0":"=m"(pinfo.registers[14]));
    __asm__ volatile("mov %%r15,%0":"=m"(pinfo.registers[15]));
}

__attribute__((no_caller_saved_registers))
static void setup_panic(const char* reason, const INTERRUPT_FRAME* frame, u64 err, u64 cr2 = 0)
{
    PANIC_INFO pinfo {};
    pinfo.reason = reason;
    pinfo.rip = frame->rip;
    pinfo.errorCode = err;
    pinfo.faultAddr = cr2;
    fill_registers(pinfo);
    
    panic::panic(pinfo);
}

extern "C" __attribute__((interrupt, no_caller_saved_registers)) 
void dummy_noerr(INTERRUPT_FRAME* frame)
{
    putc('X');
    setup_panic("CPU Exception", frame, 0);
}

extern "C" __attribute__((interrupt, no_caller_saved_registers)) 
void dummy_err(INTERRUPT_FRAME* frame, u64 err)
{
    putc('X');
    setup_panic("CPU Exception", frame, err);
}

extern "C" __attribute__((interrupt, no_caller_saved_registers)) 
void page_fault(INTERRUPT_FRAME* frame, u64 err)
{
    putc('P'); 
    uint64_t addr; 
    __asm__ volatile("mov %%cr2,%0":"=r"(addr));
    setup_panic("#PF", frame, err, addr);
}