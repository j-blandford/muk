# flags and targets
TARGET = i686-elf
ARCH = i386

CPPFLAGS  = -O2 -g -std=gnu++14 
CPPFLAGS += -nostdlib -finline-functions -ffreestanding -fno-exceptions -fpermissive
CPPFLAGS += -Warray-bounds -Wno-write-strings -Wno-unused-variable -Wno-unused-parameter -Wno-ignored-qualifiers
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

OBJS+= $(patsubst %.cpp,%.o,$(wildcard ../libcxxrt/*.cpp))

# .PHONY: printvars 
# printvars: 
# 	@$(foreach V,$(sort $(.VARIABLES)), $(if $(filter-out environment% default automatic, $(origin $V)),$(warning $V=$($V) ($(value $V)))))

.PHONY: compile run clean

.SECONDARY:

.SUFFIXES:

compile: ${OBJS}
	@${NASM} ${NASM_FLAGS} asm/boot.s
	@${GPP} -T linker.ld ${CPPFLAGS} -o ../build/boot/kernel.elf asm/boot.o ${OBJS} 
	@${INFO} "---->" "Kernel compiled..."

# This provides the object files
%.o: %.cpp
	@${GPP} ${CPPFLAGS} -g -c -I./../include -o $@ $< ${ERRORS}

# # Compiles .s (boot.s, etc.) into object files with NASM
# %.o: ${ASMOBJS}
# 	@${NASM} ${NASM_FLAGS} $@


clean:
	@-rm -f *.o
	@-rm -f */*.o
	@-rm -f */*/*.o
	@-rm -f build/boot/*.elf
