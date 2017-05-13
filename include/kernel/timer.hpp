#pragma once

#include <stddef.h>
#include <stdint.h>

extern volatile uint64_t timer_ticks;
extern volatile uint32_t timer_tenths;
extern volatile bool is_sleeping;

namespace Timer {
    void initTimer();
    void testTimer();

    void setCount(double hz);
    void sleep(int ticks);

    void yield();
};

void sleep(int ticks);
void wait(int ticks);
