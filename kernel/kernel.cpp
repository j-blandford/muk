#include <stddef.h>
#include <stdint.h>

#include <kernel/multiboot.hpp>
#include <kernel/icxxabi.hpp>
#include <kernel/tty.hpp>
#include <kernel/gdt.hpp>
#include <kernel/idt.hpp>

extern "C"
void kernel_main(multiboot_info_t * mb_info, uint32_t stack_size, uintptr_t esp) {
    init_tty();

    gdt_install();
    idt_install();

    while(true) { }
}
