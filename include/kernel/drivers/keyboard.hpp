#pragma once

#include <stddef.h>
#include <std/ringbuffer.hpp> 

#define KEYBOARD_BUFFER_SIZE 512

// unsigned char inb_kbr(uint16_t port);
// void outb_kbr(uint16_t port, uint8_t val);

extern char _keyboard_buffer[KEYBOARD_BUFFER_SIZE];
extern buffer_t keyboard_buffer;

void kb_update();

void keyboard_handler(void);
void keyboard_install(void);

char getc(); // Get Char
//char* parse_buffer(char* str, size_t max); // get user input and sent it to tty