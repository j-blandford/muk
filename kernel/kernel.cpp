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

#include <kernel/memory/physical.hpp>

extern "C"
void kernel_main(multiboot_info_t * mb_info, uint32_t k_phys_start, uint32_t k_phys_end) {
    init_tty();

    gdt_install();
    idt_install();

    // IRQ0
    Timer::initTimer();
    // Timer::testTimer();

    // IRQ1
    // kb code here

    pmm_setup(mb_info, k_phys_start, k_phys_end);
    pg_directory_setup(); // set up the page tables

    // terminal_printf("GRUB modules: \n");
    // module_t * modules;
    // if (mb_info->mods_count > 0) {
    //     modules = (module_t *) (mb_info->mods_addr + KERNEL_VIRT_BASE);
    //     for (uint8_t i = 0; i < mb_info->mods_count; i++) {
    //         terminal_printf("%s: %x->%x\n",(char*) (modules[i].string + KERNEL_VIRT_BASE), (modules[i].mod_start + KERNEL_VIRT_BASE), (modules[i].mod_end + KERNEL_VIRT_BASE));
    //     }
    // }

    void_fn module_one = get_module_funct(mb_info, 0); // should execute out "basic_program.s" file...
    terminal_printf("module_one address: %x\n", module_one);
    module_one();
    MAGIC_BREAK;

    terminal_printf("kernel_main address: %x\n", &kernel_main);

    while(true) { }
}
