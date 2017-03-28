#pragma once

#include <kernel/multiboot.hpp>
#include <kernel/gdt.hpp>
#include <kernel/idt.hpp>

#define set_kstack(stack) tss.esp0 = (uint32_t)(stack)

void set_ip(uintptr_t eip);
void schedule_next(struct registers * r);
void scheduler_init();