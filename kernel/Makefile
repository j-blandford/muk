# flags and targets
TARGET = i686-elf
ARCH = i386

CPPFLAGS  = -O2 -g -std=gnu++11 -nostdinc++ -fno-rtti
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

# Emulator flags and targets
QEMU = qemu-system-i386
QEMU_FLAGS = -s
QEMU_FLAGS += -cdrom myos.iso	# the kernel image
#QEMU_FLAGS += -drive file=disk.img,format=raw		# our basic dummy HDD image
QEMU_FLAGS += -boot d			# boot from CDROM
QEMU_FLAGS += -m 64			# 512mb should be enough
QEMU_FLAGS += -monitor stdio		# output to the current terminal
QEMU_FLAGS += -vga cirrus

OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
OBJS+= $(patsubst %.cpp,%.o,$(wildcard */*.cpp))
OBJS+= $(patsubst %.cpp,%.o,$(wildcard */*/*.cpp))

HEADERS = $(shell find include/ -type f -name '*.hpp')

.PHONY: compile run clean

.SECONDARY:

.SUFFIXES:

compile: ${OBJS}
	@${NASM} ${NASM_FLAGS} boot.s
	@${GPP} -T linker.ld ${CPPFLAGS} -o ../build/boot/kernel.elf boot.o ${OBJS}
	@${INFO} "---->" "Kernel built..."

# This provides the object files
%.o: %.cpp ${KERNEL_HEADERS}
	@${GPP} ${CPPFLAGS} -g -c -I./../include -o $@ $< ${ERRORS}

# # Compiles .s (boot.s, etc.) into object files with NASM
# boot.o:
# 	@${NASM} ${NASM_FLAGS} boot.s


clean:
	@-rm -f *.o
	@-rm -f */*.o
	@-rm -f */*/*.o
	@-rm -f build/boot/*.elf
