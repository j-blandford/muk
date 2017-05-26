#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.hpp>
#include <kernel/cpu.hpp>
#include <kernel/idt.hpp>
#include <kernel/timer.hpp>

#include <kernel/proc/scheduler.hpp>
#include <kernel/proc/thread.hpp>

volatile uint64_t timer_ticks;
volatile uint32_t timer_tenths;
volatile bool is_sleeping;

static void timer_irq(struct registers * r) {
    timer_ticks++;
    timer_tenths++;

    if(timer_tenths > 25 && !is_sleeping) {
        timer_tenths = 0;

        Scheduler::next(r);
    }

    return;
}

void Timer::setCount(double hz) {
    int count = (int)(1193182.0f / hz);

    outportb(0x43, 0x34);  // ‭"0 011 01 00" on the command port

    outportb(0x40, count & 0xFF); // low byte
    outportb(0x40, (count >> 8) & 0xFF); // high byte
}

void Timer::sleep(int ticks) {
	uint64_t eticks = timer_ticks + ticks;

    is_sleeping = true;

	while(timer_ticks < eticks)
	{
       // bcprintf("tick: %d, etick: %d\n", timer_ticks, eticks);
		__asm__ __volatile__ ("sti//hlt//cli");
	}

    is_sleeping = false;
}

void Timer::yield() {
    interrupts_enable();
    timer_tenths = 100; // force the next timer IRQ to switch threads
}

void Timer::initTimer() {
    timer_ticks = 0;
    timer_tenths = 0;
    is_sleeping = false;
    
    Timer::setCount(1000.99125);

    Interrupt::Register(0, timer_irq);

}

void Timer::testTimer() {
    for(int i = 0; i < 100; i++) {
        Timer::sleep(500);
        bcprintf("Waited 500ms\n");
        //terminal_writestring("Waited 500ms.\n");
//        update_buffer();
    }

    MAGIC_BREAK;
}

void sleep(int ticks) {
    Timer::sleep(ticks);
}
