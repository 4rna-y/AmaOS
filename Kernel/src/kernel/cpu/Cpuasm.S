.intel_syntax noprefix
.section .text
.global cpu_lgdt, cpu_lidt, cpu_load_tss_m16, cpu_far_switch, cpu_reset_tr

cpu_lgdt:
    lgdt [rdi]
    ret

cpu_lidt:
    lidt [rdi]
    ret

cpu_load_tss_m16:
    mov word ptr [rip + tss_sel], di
    ltr word ptr [rip + tss_sel]
    ret
tss_sel: .word 0

cpu_far_switch:
    push 0x08
    lea rax, [rip + 1f]
    push rax
    retfq

1:  mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    ret