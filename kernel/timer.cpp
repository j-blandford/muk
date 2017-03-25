#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.hpp>
#include <kernel/cpu.hpp>
#include <kernel/idt.hpp>
#include <kernel/timer.hpp>

#include <kernel/proc/scheduler.hpp>

volatile uint64_t timer_ticks;
volatile uint32_t timer_tenths;

static void timer_irq0(struct registers * registers) {
    timer_ticks++;
    timer_tenths++;

    if(timer_tenths > 10) {
        // __asm__ volatile("int $0x82 \n");
        // schedule();

        timer_tenths = 0;
    }

    return;
}

void Timer::setCount(int hz) {
    int count = 1193182 / hz;

    outportb(0x43, 0x34);  // ‭"0 011 01 00" on the command port

    outportb(0x40, count & 0xFF); // low byte
    outportb(0x40, (count >> 8) & 0xFF); // high byte
}

void Timer::sleep(int ticks) {
	uint64_t eticks = timer_ticks + ticks;

	while(timer_ticks < eticks)
	{
		__asm__ __volatile__ ("sti//hlt//cli");
	}
}

void Timer::initTimer() {
    timer_ticks = 0;
    timer_tenths = 0;
    
    Timer::setCount(1000);

    set_irq_handler(0, timer_irq0);

}

void Timer::testTimer() {
    for(int i = 0; i < 10; i++) {
        Timer::sleep(500);
        terminal_writestring("Waited 500ms.\n");
//        update_buffer();
    }
}

void sleep(int ticks) {
    Timer::sleep(ticks);
}

void Timer(int ticks) {
    Timer::sleep(ticks);
}
