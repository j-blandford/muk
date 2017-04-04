MB_HEADER_MAGIC equ 0x1BADB002
MB_EAX_MAGIC equ 0x2BADB002
MB_PAGE_ALIGN	equ 1<<0         ; align modules on 4kb boundaries
MB_MEMORY_INFO	equ 1<<1         ; request mem_info struct
MB_VIDEO_MODE equ 1<<2           ; request a video mode
MB_FLAGS equ (MB_PAGE_ALIGN | MB_MEMORY_INFO | MB_VIDEO_MODE)
MB_CHECKSUM equ -(MB_HEADER_MAGIC + MB_FLAGS)

KERNEL_STACK_LENGTH equ 32768

KERNEL_VIRTUAL_OFFSET equ 0xC0000000
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_OFFSET >> 22) ; this is what the kernel's offset is in the page directory = 768

; below are the functions+sections from .ld that we have to declare here to use them in our kernel boot process
extern kernel_main
extern kernel_virt_start, kernel_virt_end
extern kernel_phys_start, kernel_phys_end
extern code
extern bss
extern end

global bootstrapper

bits 32                          ; 32 bit at the moment

section .mb_header               ; multiboot header (for GRUB)
mb_header:
    dd MB_HEADER_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM
    dd mb_header                ; header_addr

    dd code                     ; load_addr
    dd bss                      ; load_end
    dd end                      ; load_bss_end
    dd bootstrapper                   ; entry_addr

    dd 1                        ; 0 = vbe mode, 1 = linear framebuffer
    dd 640
    dd 480
    dd 24

; thanks to Connor Stack for the tutorial on higher-half implementation!
section .data
align 0x1000
global PDVirtualAddress
PDVirtualAddress:
    ; 0x83 = 10000011 (bit 7, 1, 0 are all set)
    ; used as a temporary map for our kernel so that after paging is enabled,
    ; we can keep the program running!
    dd 0x00000083   ; [4MB page] | [r/w] | [present]

    times (KERNEL_PAGE_NUMBER -1) dd 0 ; blank pages before the 4mb on

    dd 0x00000083   ; page entry for kernel 4mb page

    times (1024 - KERNEL_PAGE_NUMBER -1) dd 0 ; entry for rest of pages


section .text                   ; code section start
align 4

_bootstrapper:
bootstrapper equ (_bootstrapper - KERNEL_VIRTUAL_OFFSET)
    ; first, we enable 4mb pages in CR4
    ; this allows us to be more efficient in our code, as we would have to set each 4kb page manually otherwise!
    mov eax, cr4
    or eax, 0x00000010              ; which is bit 7
    mov cr4, eax

    ; then set the paging pointer in cr3
    global PDPhysicalAddress
    PDPhysicalAddress equ (PDVirtualAddress - KERNEL_VIRTUAL_OFFSET)
    mov eax, PDPhysicalAddress
    mov cr3, eax

    ; now we can enable the paging bit of CR0 (this will enable paging from HERE - be careful about addresses!)
    mov eax, cr0
    or eax, 0x80000000              ; which is bit 31
    mov cr0, eax

    lea ecx, [_boot]
    jmp ecx

_boot:
    cli                         ; disable interrupts at the moment
    finit

    ; Succesfully moved the kernel into the upper half :)
    mov dword [PDVirtualAddress], 0
    ; we have an extra page-table (0) that we can now remove.
    invlpg [0]

    add ebx, KERNEL_VIRTUAL_OFFSET ; ebx contains our GRUB pointer, which needs to be a virtualised address
    mov esp, kstack               ; setup stack pointer (end of memory area)
    
    push kernel_phys_end
    push kernel_phys_start
    push ebx                    ; multiboot header struct (param 1)
    
    call kernel_main            ; now we can call our main kernel function

    ; hang the computer if kernel main loop returns
    cli
    hlt

.end:


%include "asm/gdt.s"
%include "asm/idt.s"
%include "asm/interrupts.s"
%include "asm/memory.s"

global kstack
section .bss
    align 16
    kstack:
        resb KERNEL_STACK_LENGTH
