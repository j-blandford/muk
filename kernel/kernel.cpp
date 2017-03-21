#include <stddef.h>
#include <stdint.h>

#include <kernel/multiboot.hpp>
#include <kernel/icxxabi.hpp>
#include <kernel/tty.hpp>
#include <kernel/gdt.hpp>
#include <kernel/idt.hpp>
#include <kernel/timer.hpp>

#include <kernel/memory/physical.hpp>

extern "C"
void kernel_main(multiboot_info_t * mb_info, uint32_t stack_size, uintptr_t esp) {
    init_tty();

    gdt_install();
    idt_install();

    // IRQ0
    Timer::initTimer();
    // Timer::testTimer();

    // IRQ1
    // kb code here

    pmm_setup(mb_info);

    //uint32_t a = pmm_allocate();
    // uint32_t b = pmm_allocate();
    // uint32_t c = pmm_allocate();
    // pmm_free(b);
    // uint32_t d = pmm_allocate();

    // terminal_printf("\na:%x\nb:%x\nc:%x\nd:%x",a,b,c,d);


    terminal_printf("kernel_main address: %x\n", &kernel_main);


    while(true) { }
}
