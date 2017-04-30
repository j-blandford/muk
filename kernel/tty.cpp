#include <stddef.h>
#include <stdint.h>

#include <std.hpp>

#include <kernel/cpu.hpp>
#include <kernel/tty.hpp>
#include <kernel/command.hpp>
#include <kernel/memory/alloc.hpp>
#include <kernel/drivers/keyboard.hpp>
#include <kernel/user/env.hpp>
#include <kernel/proc/message.hpp>
#include <kernel/proc/scheduler.hpp>
#include <kernel/gfx/buffer.hpp>
#include <kernel/gfx/surface.hpp>

volatile size_t terminal_row = 0;
volatile size_t terminal_column = 0;
uint16_t* terminal_buffer;

const int X_FONTWIDTH = 8;
const int Y_FONTWIDTH = 16; 

static Graphics::Vector2 cursor_pos(0,0);

char kb_buffer[1024];

void terminal_writestring(char* data, Graphics::RGB colour) {
	// if(terminal_row == VGA_HEIGHT) {
	// //	terminal_scrollup();
	// }
	//Scheduler::lock();
	for (; *data; ++data) {
		terminal_putchar((*data), colour);
	}
	//Scheduler::unlock();
}

void terminal_putentryat(char c, size_t x, size_t y, Graphics::RGB colour) {
	const size_t index = y * VGA_WIDTH + x;
	//terminal_buffer[index] = vga_entry(c, VGA_COLOR_WHITE);
	drawchar_transparent(c, x*X_FONTWIDTH, (y+1)*Y_FONTWIDTH, colour);
}

void terminal_putchar(char c, Graphics::RGB colour) {
	if(c == '\n') {
		terminal_column = 0;
		terminal_row++;

		return;
	}

	if(terminal_row == VGA_HEIGHT) {
		terminal_scrollup();
	}

	if(c == '\t') {
		// support tabs
		terminal_column += TAB_WIDTH - terminal_column % TAB_WIDTH;
		return;
	}

	terminal_putentryat(c, terminal_column, terminal_row, colour);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;

		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}

	update_cursor(terminal_row, terminal_column);
}

void terminal_printf(const char* fmt, ...) {
	va_list parameters;
	char temp_buffer[80] = {0};

	va_start(parameters, fmt);
	vsprintf(temp_buffer, fmt, parameters);
	terminal_writestring((char*)temp_buffer);
	va_end(parameters);
}

void terminal_clearline(size_t y) {
	// for (size_t x = 0; x < VGA_WIDTH; x++) {
	// 	const size_t index = y * VGA_WIDTH + x;
	// 	terminal_buffer[index] = vga_entry(' ', terminal_color);
	// }
}

void terminal_scrollup() {
	terminal_row = VGA_HEIGHT-1;

	for(size_t y = 1; y <= VGA_HEIGHT; y++) {
		int dest_idx = (y-1)*VGA_WIDTH;
		int src_idx = (y)*VGA_WIDTH;

		terminal_clearline(y-1);

		//memcpy(&terminal_buffer[dest_idx], &terminal_buffer[src_idx], VGA_WIDTH * sizeof(uint16_t));
	}
}

void update_cursor(int row, int col) {
	unsigned short position=(row*80) + col;

	cursor_pos.x = col+1;
	cursor_pos.y = row;
}

size_t tty_get_cursor_x() {
	return terminal_column;
}

void tty_set_cursor_x(size_t x) {
	terminal_column = x;

	update_cursor(terminal_row, x);
}

/**
*	This function is used for the "tty_driver" thread
*/
void tty_update() {
	// Process::Message msg = Process::kMessageNull;

	// {
	// 	using namespace Process;
	// 	Locker<SpinlockMutex> messaging_locker(messaging_mutex, Scheduler::threadId());
	// 	Process::listen(2);
	// }

	bcprintf("FIRST ");

	for(;;) {
		// bool parsing = false;
		// size_t c_idx = 0;

		// terminal_writestring("\n[", Graphics::RGB(0xe4e4c8));
		// terminal_writestring("james", Graphics::RGB(0xff6064));
		// terminal_writestring("@", Graphics::RGB(0xff6064));
		// terminal_writestring("localhost", Graphics::RGB(0xff6064));
		// terminal_writestring("0:", Graphics::RGB(0x288acc));
		// terminal_writestring("/", Graphics::RGB(0x288acc));
		// terminal_writestring("] ", Graphics::RGB(0xe4e4c8));

		// for(;;) {
		// 	while((msg = Process::postbox.pop(2)) != Process::kMessageNull) {
		// 		bcprintf("TTY receiving data '%c'\n", msg.data);
		// 		terminal_putchar(msg.data);
		// 	}
		// }


		bcprintf("two ");



		// while(!parsing) {

		// 	for(size_t idx = 0; idx < keyboard_buffer.size; idx++) {
		// 		if(keyboard_buffer.buffer[idx] == 28) {
		// 			parsing = true;
		// 			break;
		// 		} 
		// 		else {
		// 			command[c_idx] = keyboard_buffer.buffer[idx];

		// 			// char to_print = command[c_idx];
		// 			// if(to_print != 0) {
		// 			// 	bcprintf("Printing char '%c'\n",to_print);
		// 			// 	terminal_putchar(to_print);
		// 			// }

		// 			parsing = false;
		// 			c_idx++;
		// 		}
		// 	}

		// }
		// buffer_clear(&keyboard_buffer);

		// Command::Parse(kb_buffer);
	}
}