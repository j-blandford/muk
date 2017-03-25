#pragma once

#include <kernel/multiboot.hpp>
#include <kernel/gdt.hpp>

#define set_kstack(stack) tss.esp0 = (uint32_t)(stack)

void schedule();
void scheduler_init();