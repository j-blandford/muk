MB_HEADER_MAGIC equ 0x1BADB002
MB_EAX_MAGIC equ 0x2BADB002
MB_PAGE_ALIGN	equ 1<<0         ; align on 4kb boundary
MB_MEMORY_INFO	equ 1<<1         ; request mem_info struct
MB_VIDEO_MODE equ 1<<2           ; request a video mode
MB_FLAGS equ (MB_PAGE_ALIGN | MB_MEMORY_INFO | MB_VIDEO_MODE)
MB_CHECKSUM equ -(MB_HEADER_MAGIC + MB_FLAGS)

STACK_LENGTH equ 32768

; below are the functions+sections from .ld that we have to declare here to use them in our kernel boot process
extern kernel_main, code, bss, end

bits 32                          ; 32 bit at the moment

section .mb_header               ; multiboot header (for GRUB)
mb_header:
    dd MB_HEADER_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

    dd mb_header
    dd code
    dd kernel_main
    dd bss
    dd end
    dd __boot

    dd 1                        ; 0 = vbe mode, 1 = console mode
    dd 640
    dd 480
    dd 24

section .text
global __boot:function __boot.end-__boot
__boot:
    cli
    finit
    mov esp, stack ; setup stack pointer

    push stack      ; stack pointer (param 3)
    push STACK_LENGTH      ; stack size (param 2)
    push ebx        ; multiboot header struct (param 1)
    call kernel_main ; now we can call our main kernel function

    cli

.end:

section .bss
align 16
  resb STACK_LENGTH
stack:
