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
	Process::Message msg = Process::kMessageNull;
	char tty_buffer[1024];
	int tty_idx = 0;

	{
		using namespace Process;
		Locker<SpinlockMutex> messaging_locker(messaging_mutex, Scheduler::threadId());
		Process::listen(2);
	}

	for(;;) {
		bool parsing = false;

		memset(&tty_buffer, 0, sizeof(char)*1024);

		terminal_writestring("\n[", Graphics::RGB(0xe4e4c8));
		terminal_writestring("james", Graphics::RGB(0xff6064));
		terminal_writestring("@", Graphics::RGB(0xff6064));
		terminal_writestring("localhost", Graphics::RGB(0xff6064));
		terminal_writestring("0:", Graphics::RGB(0x288acc));
		terminal_writestring("/", Graphics::RGB(0x288acc));
		terminal_writestring("] ", Graphics::RGB(0xe4e4c8));

		while(!parsing) {

			while((msg = Process::postbox.pop(2)) != Process::kMessageNull) {
				tty_buffer[tty_idx] = msg.data;
				tty_idx++;
	
				if(msg.data != '\n')
					terminal_putchar(msg.data);
			}

			for(size_t idx = 0; idx < 1024; idx++) {
				if(tty_buffer[idx] == '\n') {
					parsing = true;
					tty_idx = 0;
					break;
				} 				
			}
		}

		parse((char*)&tty_buffer);
	}
}


// this function does NOT belong here - TODO create an extendable method for commands
static void ps_p(int proc_id) {
	for(Thread* t = thread_root; t->next != thread_root; t = t->next)
		//if(t->proc_id == proc_id)
		terminal_printf("%d\t%d\t00:00:01\t%s\n",t->proc_id,t->thread_id, t->title);
}
// this function does NOT belong here - TODO create an extendable method for commands
static void ps(std::vector<std::string>  args) {
	terminal_writestring("PID\tTID\tTIME\tSTR\n");

	for(int i = 1; i < args.size(); i++) {
		if(strncmp((char*)args[i], "-p", 2) == 0) {
			ps_p(atoi((char*)args[i+1]));
		}
	}
}

// static void ls(std::vector<string> args) {
// 	//						this zero should be "ENV::get(drive)"    VV 

// 	string currentPath;

// 	// Is this an absolute or relative path? 
// 	// we need to add the current working directory if it's relative
// 	if(args[1][0] == '/') {
// 		// absolute path, do nothing to it :)
// 		currentPath = args[1];
// 	} 
// 	else {
// 		// Relative path
// 		currentPath = ENV::get("cd");
// 		currentPath += args[1];
// 		currentPath += '/';
// 	}

// 	std::vector<Filesystem::DirectoryEntry> dir = Filesystem::devices[0]->readDirectory(currentPath);

// 	//terminal_writestring("PID\tTID\tTIME\tSTR\n");

// 	for(auto it = dir.begin(); it != dir.end(); it++) {
// 		char* fileType = new char[5];

// 		if((*it).attributes == Filesystem::FATAttributes::shortNameFile) {
// 			strncpy(fileType, "FILE", 5);
// 		} 
// 		else {
// 			strncpy(fileType, "DIR", 4);
// 		}
		
// 		terminal_printf("%s ", (*it).name);

// 		if(strlen((*it).name) < 11) {
// 			terminal_writestring("\t");
// 		}

// 		terminal_printf("\t %s @ %x\n", fileType, (*it).location);
// 	}
// }

void parse(char* buffer) {
	std::vector<std::string> args;
	std::string str = buffer;

	Scheduler::lock();
	
	for (auto i = strtok(str.data(), " "); i != nullptr; i = strtok(nullptr, " ")) {
		args.push_back(std::string(i));
	}

	terminal_writestring("\n");

	ps(args);

	Scheduler::unlock();
}