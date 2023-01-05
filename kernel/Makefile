CC = $(HOME)/opt/cross/bin/i686-elf-gcc
CXX = $(HOME)/opt/cross/bin/i686-elf-g++
ASM = nasm
ASFLAGS:= -f elf
CPPFLAGS:=$(CPPFLAGS) -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-plt -fno-pic -m32
LDFLAGS:=$(LDFLAGS) -T linker.ld -ffreestanding -O2 -nostdlib -lgcc

CRTI_OBJ=crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJ=crtn.o

OBJECTS =  boot.o serial.o kernel.o video.o utils/string.o paging.o
# OBJECTS = boot.o kernel.o paging.o



OBJECTS_LINK_LIST:=$(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJECTS) $(CRTEND_OBJ) $(CRTN_OBJ)
INTERNAL_OBJS:=$(CRTI_OBJ) $(OBJECTS) $(CRTN_OBJ)


.PHONY: clean

all: kernel.elf

kernel.elf: $(OBJECTS_LINK_LIST) linker.ld
	$(CC) $(LDFLAGS) $(OBJECTS_LINK_LIST) -o $@
	grub-file --is-x86-multiboot $@

crtbegin.o crtend.o:
	OBJ=`$(CC) $(CPPFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

%.o: %.cpp %.h
	$(CXX) -c $(CPPFLAGS) $< -o $@

%.o: %.s
	$(ASM) $(ASFLAGS)  $< -o $@

initrd.o: initrd.s ../initrd/initrd.tar
	$(ASM) $(ASFLAGS)  $< -o $@

clean:
	rm -rf $(INTERNAL_OBJS) kernel.elf
