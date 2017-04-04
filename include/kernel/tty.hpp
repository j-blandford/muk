#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#include <kernel/gfx/vga.hpp>
#include <kernel/gfx/vbe.hpp>

#define TAB_WIDTH 12

extern size_t terminal_row;
extern size_t terminal_column;
extern uint8_t terminal_color;
extern uint16_t* terminal_buffer;

void init_tty();
void tty_update();
void terminal_scrollup();
void terminal_clearline(size_t y);
void terminal_writestring(const char* data);
void terminal_putentryat(const char c, size_t x, size_t y);
void terminal_putchar(const char c);

void terminal_printf(const char* fmt, ...);

void update_cursor(int row, int col);

size_t tty_get_cursor_x();
void tty_set_cursor_x(size_t x);