#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;


void init_tty() {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
	terminal_writestring("[TTY] Initialised");
}

void terminal_writestring(const char* data) {
	if(terminal_row == VGA_HEIGHT) {
	//	terminal_scrollup();
	}

	for (; *data; ++data) {
		terminal_putchar((*data));
	}
}

void terminal_putentryat(const char c, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;

	terminal_buffer[index] = vga_entry(c, VGA_COLOR_WHITE);

	// drawchar_transparent(c, x*X_FONTWIDTH, y*Y_FONTWIDTH, color);
}

void terminal_putchar(const char c) {
	if(c == '\n') {
		terminal_column = 0;
		terminal_row++;
		return;
	}

	// if(c == '\t') {
	// 	// support tabs
	// 	terminal_column += TAB_WIDTH - terminal_column % TAB_WIDTH;
	// 	return;
	// }

	terminal_putentryat(c, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}

	update_cursor(terminal_row, terminal_column);
}

void update_cursor(int row, int col) {
	unsigned short position=(row*80) + col;

	// cursor_pos.x = col+1;
	// cursor_pos.y = row;

	// // cursor LOW port to vga INDEX register
	// outportb(0x3D4, 0x0F);
	// outportb(0x3D5, (unsigned char)(position&0xFF));
	//
	// // cursor HIGH port to vga INDEX register
	// outportb(0x3D4, 0x0E);
	// outportb(0x3D5, (unsigned char )((position>>8)&0xFF));

}
