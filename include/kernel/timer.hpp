#pragma once

#include <stddef.h>
#include <stdint.h>

extern volatile uint64_t timer_ticks;

class Timer {
public:
    static void initTimer();
    static void testTimer();

    static void setCount(int hz);
    static void sleep(int ticks);
};

void sleep(int ticks);
void wait(int ticks);
