#include <stddef.h>
#include <stdint.h>

#include <kernel/multiboot.hpp>
#include <kernel/icxxabi.hpp>
#include <kernel/cpu.hpp>
#include <kernel/module.hpp>
#include <kernel/tty.hpp>
#include <kernel/gdt.hpp>
#include <kernel/idt.hpp>
#include <kernel/timer.hpp>

#include <kernel/proc/scheduler.hpp>

#include <kernel/memory/physical.hpp>

extern "C"
void kernel_main(multiboot_info_t * mb_info, uint32_t k_phys_start, uint32_t k_phys_end) {
    init_tty();

    gdt_install();
    tss_install();
    
    idt_install();

    // IRQ0
    Timer::initTimer();
    Timer::testTimer();

    // IRQ1
    // kb code here

    pmm_setup(mb_info, k_phys_start, k_phys_end);
    pg_directory_setup(); // set up the page tables

    // ISR130
    scheduler_init();

    // void_fn module_one = get_module_funct(mb_info, 0); // should execute our "basic_program.s" file...
    // terminal_printf("module_one address: %x\n", module_one);
    // MAGIC_BREAK;
    // module_one();

    terminal_printf("kernel_main address: %x\n", &kernel_main);

    while(true) { }
}
