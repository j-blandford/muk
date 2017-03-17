#include <stddef.h>
#include <stdint.h>

#include <kernel/multiboot.hpp>
#include <kernel/icxxabi.hpp>
#include <kernel/tty.hpp>
#include <kernel/gdt.hpp>
#include <kernel/idt.hpp>
#include <kernel/timer.hpp>

extern "C"
void kernel_main(multiboot_info_t * mb_info, uint32_t stack_size, uintptr_t esp) {
    init_tty();

    gdt_install();
    idt_install();

    // IRQ0
    Timer::initTimer();
//    Timer::testTimer();

    // IRQ1
    // kb code here

    terminal_printf("Memory Address: %x\n", &kernel_main);

    while(true) { }
}
