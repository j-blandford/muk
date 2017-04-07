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
#include <kernel/fs.hpp>
#include <kernel/drivers/keyboard.hpp>
#include <kernel/fs/fat16.hpp>
#include <kernel/proc/scheduler.hpp>
#include <kernel/proc/process.hpp>
#include <kernel/memory/physical.hpp>
#include <kernel/user/env.hpp>
#include <kernel/gfx/surface.hpp>

extern "C"
void kernel_main(multiboot_info_t * mb_info, uint32_t k_phys_start, uint32_t k_phys_end) {

    init_tty();

    gdt_install();
    tss_install();
    idt_install();

    pmm_setup(mb_info, k_phys_start, k_phys_end);
    page_directory_t kernel_pd = pg_directory_setup(); // set up the page tables

    // uint32_t* alloc1 = (uint32_t*)kmalloc(sizeof(uint32_t));
    // bcprintf("&alloc1=%x\n", alloc1);
    // MAGIC_BREAK;
    // uint32_t* alloc2 = (uint32_t*)kmalloc(sizeof(uint32_t)*2);
    // bcprintf("&alloc2=%x\n", alloc2);
    // MAGIC_BREAK;
    // uint32_t* alloc3 = (uint32_t*)kmalloc(sizeof(uint32_t)*2000);
    // bcprintf("&alloc3=%x\n", alloc3);
    // MAGIC_BREAK;
    // uint32_t* alloc4 = (uint32_t*)kmalloc(sizeof(uint32_t)*500);
    // bcprintf("&alloc4=%x\n", alloc4);
    // bcprintf("got to bottom :)\n");

    start_display_driver(mb_info);
    
    // // IRQ0
    // Timer::initTimer();
    // // IRQ1
    // keyboard_install();

    // ENV::initialise();

    // // ISR130
    // // scheduler_init();

    // // void_fn module_one = get_module_funct(mb_info, 0); // should execute our "basic_program.s" file...
    // // terminal_printf("module_one address: %x\n", module_one);
    // // MAGIC_BREAK;
    // // module_one();

    // interrupts_enable();

    // init_kthreads();

    // Filesystem::initialise();

    // terminal_writestring("\n");
    // // This does nothing apart from stop our kmain function from returning
    // // Every process is now a thread (running almost asynchronously)
    while(true) { }
}
