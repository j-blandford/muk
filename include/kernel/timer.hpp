#pragma once

#include <stddef.h>
#include <stdint.h>

extern volatile uint64_t timer_ticks;
extern volatile uint32_t timer_tenths;

namespace Timer {
    void initTimer();
    void testTimer();

    void setCount(int hz);
    void sleep(int ticks);

    void yield();
};

void sleep(int ticks);
void wait(int ticks);
