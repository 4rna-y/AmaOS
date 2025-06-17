/* linker.ld — PIE/再配置対応版 */

OUTPUT_FORMAT("elf64-x86-64");
OUTPUT_ARCH(i386:x86-64);
ENTRY(k_main)

PHDRS 
{
  text     PT_LOAD     FLAGS(5);  
  data     PT_LOAD     FLAGS(6);  
  dyn      PT_DYNAMIC  FLAGS(6);  
  rel      PT_LOAD     FLAGS(4); 
}

SECTIONS 
{
  . = 0;
  .text    : { *(.text*)    } :text
  .rodata  : { *(.rodata*)  } :text
  .dynamic : { *(.dynamic)   } :dyn
  .rela.dyn : { *(.rela.dyn) } :rel
  .rela.plt : { *(.rela.plt) } :rel
  .data    : { *(.data*)     } :data
  .bss (NOLOAD) : { *(.bss*)  } :data
}