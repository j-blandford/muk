# flags and targets
TARGET = i686-elf
ARCH = i386

CPPFLAGS  = -O2 -g -std=gnu++11 -nostdinc++ -fno-rtti
CPPFLAGS += -finline-functions -ffreestanding -nostdlib
CPPFLAGS += -Wall -Wextra -fno-exceptions -Warray-bounds
CPPFLAGS += -Wno-write-strings -Wno-unused-variable -Wno-unused-parameter
CPPFLAGS += -DKERNEL

NASM = nasm
NASM_FLAGS = -f bin		# output in raw BIN format (for the moment...)
NASM_FLAGS+= -g 		# include debug information

GCC = ${TARGET}-gcc
GPP = ${TARGET}-g++

INFO = ../scripts/print-msg.sh

OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
OBJS+= $(patsubst %.cpp,%.o,$(wildcard */*.cpp))
OBJS+= $(patsubst %.cpp,%.o,$(wildcard */*/*.cpp))

HEADERS = $(shell find include/ -type f -name '*.hpp')

ASMOBJS = $(patsubst %.s,%.bin,$(wildcard *.s))

.PHONY: compile clean

.SECONDARY:

.SUFFIXES:

compile: ${ASMOBJS}
	cp -r *.bin ../build/modules
	@${INFO} "---->" "Modules built..."

# 	@${GPP} -T linker.ld ${CPPFLAGS} -o ../build/boot/kernel.elf asm/boot.o ${OBJS}
# This provides the object files
# %.o: %.cpp ${KERNEL_HEADERS} ${ASMOBJS}
# 	@${GPP} ${CPPFLAGS} -g -c -I./../include -o $@ $< ${ERRORS}

# # Compiles .s (boot.s, etc.) into object files with NASM
%.bin: %.s
	@${NASM} ${NASM_FLAGS} $^ -o $@


clean:
	@-rm -f *.bin
	@-rm -f */*.bin
	@-rm -f */*/*.bin
	@-rm -f build/modules/*.bin
	@-rm -f build/modules/*.elf
