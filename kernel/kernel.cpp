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

#include <kernel/drivers/keyboard.hpp>

#include <kernel/proc/scheduler.hpp>
#include <kernel/proc/process.hpp>

#include <kernel/memory/physical.hpp>

#include <kernel/user/env.hpp>

extern "C"
void kernel_main(multiboot_info_t * mb_info, uint32_t k_phys_start, uint32_t k_phys_end) {
    init_tty();

    gdt_install();
    tss_install();

    idt_install();

    // IRQ0
    Timer::initTimer();
    // IRQ1
    keyboard_install();

    pmm_setup(mb_info, k_phys_start, k_phys_end);
    page_directory_t kernel_pd = pg_directory_setup(); // set up the page tables

    ENV::setup();

    // ISR130
    // scheduler_init();

    // void_fn module_one = get_module_funct(mb_info, 0); // should execute our "basic_program.s" file...
    // terminal_printf("module_one address: %x\n", module_one);
    // MAGIC_BREAK;
    // module_one();

    // terminal_printf("kernel_main address: %x\n\n", &kernel_main);

    interrupts_enable();

    init_kthreads();

    terminal_writestring("\n");

    // This does nothing apart from stop our kmain function from returning
    // Every process is now a thread (running almost asynchronously)
    while(true) { }
}
