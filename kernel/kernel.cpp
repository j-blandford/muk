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
#include <kernel/memory/manager.hpp>
#include <kernel/user/env.hpp>
#include <kernel/gfx/surface.hpp>

#include <kernel/gfx/terminus.hpp>

extern "C"
void kernel_main(multiboot_info_t* mb_info, uint32_t k_phys_start, uint32_t k_phys_end) {
    gdt_install();
    tss_install();
    idt_install();

    // Set up the Memory manager (botrh physical and virtual)
    Memory::Setup(mb_info, k_phys_end);

    // IRQ0
    Timer::initTimer();
    // IRQ1
    keyboard_install();

    
    std::string buffer = "hello world lol cano";

    bcprintf("Finding ' ': %d (exp. 5)\n", buffer.find(" "));
    bcprintf("Finding 2nd ' ': %d (exp. 11)\n", buffer.find(" ", 6));
    bcprintf("Finding 'a': %d (exp. 17)\n", buffer.find("a"));
    bcprintf("Finding 'w': %d (exp. 6)\n", buffer.find("w"));
    bcprintf("Finding 'world': %d (exp. 6)\n", buffer.find("world"));
    
    for(;;);


    // this loads our driver threads into the scheduler
    init_kthreads();
    
    start_display_driver(mb_info);

    ENV::initialise();

    // this line starts to run the threads and drivers, the LAST thing to enable!!!
    interrupts_enable();

    // This does nothing apart from stop our kmain function from returning
    // Every process is now a thread (running "asynchronously")
    while(true) { }
}
