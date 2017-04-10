#include <stddef.h> 
#include <std/ringbuffer.hpp> 

#include <kernel/tty.hpp> 
#include <kernel/idt.hpp> 
#include <kernel/cpu.hpp> 
#include <kernel/proc/scheduler.hpp> 
#include <kernel/drivers/keyboard.hpp> 

static unsigned char keyboard_map[128] =  {
	0, 27, 
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 
	'-', '=', 
	'\b', // backspace
	'\t', // tab
	'q', 'w', 'e', 'r', 
	't', 'y', 'u', 'i', 'o', 'p', 
	'[', ']', '\n', // newline
	0, // control
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', 
	'\'', '`', 0,
	'\\', 'z', 'x', 'c', 'v', 'b', 'n',
	'm', ',', '.', '/', 0,
	'*',
	0, // alt
	' ', // space
	0, // caps lock
	0, // f1
	0, 0, 0, 0, 0, 0, 0, 0,
	0, // f10
	0, // numlock
	0, // scroll lock
	0, // home key
	0, // up arrow
	0, // page up
	'-',
	0, // left arrow
	0,
	0, // right arrow
	'+',
	0, // end key
	0, // down arrow
	0, // page down
	0, // insert key
	0, // delete key
	0, 0, 0,
	0, // f11
	0, // f12
	0, // TODO: implement the rest of em
	// 92:
	0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

static uint8_t _keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static buffer_t keyboard_buffer = { .head = 0, .tail = 0, .size = KEYBOARD_BUFFER_SIZE, .buffer = _keyboard_buffer};

char getc() {
	volatile char c;
	for (;;) {

		c = buffer_read(&keyboard_buffer);
		if (c == -1) {
			__asm__ __volatile__ ("hlt"); // wait a bit
		} 
		else {
			bcprintf("Pressed key '%c' (%d)\n", c, (int)c);

			return c;
		}
	}
}

char * getsn(char * str, size_t max) {
	
	size_t i = 0;
	while (i < max) {
		uint8_t px = tty_get_cursor_x();
		char c = getc();
		
		if (c == '\n') {
			str[i] = 0;
			bcprintf("sending buffer to command processor...\n");
			return str;
		} 
		else if (c == '\b') {
			if (i > 0) {
				terminal_putchar(' ');
				tty_set_cursor_x(tty_get_cursor_x()-2);
				terminal_putchar(' ');
				tty_set_cursor_x(tty_get_cursor_x()-2);
				terminal_putchar(' ');
				tty_set_cursor_x(tty_get_cursor_x()-1);
				i = i - 1;
			} 
			else {
				uint8_t x = tty_get_cursor_x();
				if (x < px) {
					tty_set_cursor_x(px-1);
				}
			}
		} 
		else {
			str[i] = c;
			i++;
		}
	}
	return NULL;
}

static void keyboard_irq1(registers *r) {
	uint8_t status;
	char keycode;
	
	status = inportb(0x64);

	if (status & 0x01) {

		keycode = inportb(0x60);
		if (keycode < 0) {
			return;
		}

		char c = (char)keyboard_map[(uint8_t)keycode];
		buffer_write(&keyboard_buffer, c);

		if((int)c != 0) { 
			terminal_putchar(c);
			// update_buffer(false);
		}
	}
	return;
}

void keyboard_install() {
    set_irq_handler(1, (isr_t)&keyboard_irq1); // install the keyboard irq handler
}