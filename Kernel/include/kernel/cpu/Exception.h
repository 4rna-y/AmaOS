#pragma once
#include "Types.h"
#include "panic/PanicInfo.h"

extern "C" __attribute__((interrupt, no_caller_saved_registers)) 
void dummy_noerr(INTERRUPT_FRAME* frame);
extern "C" __attribute__((interrupt, no_caller_saved_registers)) 
void dummy_err(INTERRUPT_FRAME* frame, u64 err);
extern "C" __attribute__((interrupt, no_caller_saved_registers)) 
void page_fault(INTERRUPT_FRAME* frame, u64 err);