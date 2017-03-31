# flags and targets
TARGET = i686-elf
ARCH = i386

CPPFLAGS  = -O2 -g -std=gnu++11 -fno-rtti #-nostdinc++ 
CPPFLAGS += -finline-functions -ffreestanding -nostdlib
CPPFLAGS += -Wall -Wextra -fno-exceptions -Warray-bounds
CPPFLAGS += -Wno-write-strings -Wno-unused-variable -Wno-unused-parameter
CPPFLAGS += -DKERNEL

NASM = nasm
NASM_FLAGS = -f elf		# output in ELF format
NASM_FLAGS+= -g 		# include debug information

GCC = ${TARGET}-gcc
GPP = ${TARGET}-g++

INFO = ../scripts/print-msg.sh

OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
OBJS+= $(patsubst %.cpp,%.o,$(wildcard */*.cpp))
OBJS+= $(patsubst %.cpp,%.o,$(wildcard */*/*.cpp))

HEADERS = $(shell find include/ -type f -name '*.hpp')
HEADERS+= $(shell find include/stlport/ -type f)

ASMOBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

.PHONY: compile run clean

.SECONDARY:

.SUFFIXES:

compile: ${OBJS}
	@${NASM} ${NASM_FLAGS} asm/boot.s
	@${GPP} -T linker.ld ${CPPFLAGS} -o ../build/boot/kernel.elf asm/boot.o ${OBJS}
	@${INFO} "---->" "Kernel built..."

# This provides the object files
%.o: %.cpp ${KERNEL_HEADERS} ${ASMOBJS}
	@${GPP} ${CPPFLAGS} -g -c -I./../include -o $@ $< ${ERRORS}

# # Compiles .s (boot.s, etc.) into object files with NASM
# %.o: ${ASMOBJS}
# 	@${NASM} ${NASM_FLAGS} $@


clean:
	@-rm -f *.o
	@-rm -f */*.o
	@-rm -f */*/*.o
	@-rm -f build/boot/*.elf
