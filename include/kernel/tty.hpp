#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#include <kernel/gfx/vga.hpp>
#include <kernel/gfx/vbe.hpp>

#define TAB_WIDTH 12

extern volatile size_t terminal_row;
extern volatile size_t terminal_column;
extern uint16_t* terminal_buffer;

void init_tty();
void tty_update();
void terminal_scrollup();
void terminal_clearline(size_t y);
void terminal_writestring(char* data, RGB colour = RGB(0xFFFFFF));
void terminal_putentryat(char c, size_t x, size_t y, RGB colour = RGB(0xFFFFFF));
void terminal_putchar(char c, RGB colour = RGB(0xFFFFFF));

void terminal_printf(const char* fmt, ...);

void update_cursor(int row, int col);

size_t tty_get_cursor_x();
void tty_set_cursor_x(size_t x);