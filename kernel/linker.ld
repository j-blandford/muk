ENTRY(start)

SECTIONS
{
      /* put the multiboot header FIRST so it sits at < 8KiB for GRUB to locate it */
      .mb_header : {
        *(.mb_header)
      }

      . = 0xC0100000; /* 3GiB + 1MiB kernel load (Higher Half kernel) */
      kernel_virt_start = .;
      kernel_phys_start = . - 0xC0000000;

      start = .;

      .text ALIGN(0x1000) : AT(ADDR(.text) - 0xC0000000)
      {
        code = .;
        *(.text)
      }

      .rodata ALIGN(0x1000) : AT(ADDR(.rodata) - 0xC0000000)
      {
        *(.rodata)
      }

      .data ALIGN(0x1000) : AT(ADDR(.data) - 0xC0000000)
      {
        *(.data)
      }

      .bss ALIGN(0x1000) : AT(ADDR(.bss) - 0xC0000000)
      {
        bss = .;
        *(COMMON)
        *(.bss)
      }

      end = .;

      kernel_virt_end = .;
      kernel_phys_end = . - 0xC0000000;
}
