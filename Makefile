BOOT = boot.s
KERNEL = kernel/kernel.c
LINKER = linker.ld
GRUBCONFIG = grub.cfg

SRC_DIR = src
BUILD_DIR = build
ISO_DIR = $(BUILD_DIR)/isodir

LIBC_SRC = libc/src
DISPLAY = $(LIBC_SRC)/display.c
STRING = $(LIBC_SRC)/string.c
INPUT = $(LIBC_SRC)/input.c
BSHELL = $(LIBC_SRC)/bshell.c
MEMORY = $(LIBC_SRC)/memory.c

.PHONY: build run clean

build:
	mkdir -p $(BUILD_DIR)

	i686-elf-as $(SRC_DIR)/$(BOOT) \
		-o $(BUILD_DIR)/boot.o

	i686-elf-gcc -c $(SRC_DIR)/$(KERNEL) \
		-o $(BUILD_DIR)/kernel.o \
		-std=gnu99 \
		-ffreestanding \
		-O2 \
		-Wall \
		-Wextra \
		-I$(SRC_DIR)/libc/include

	i686-elf-gcc -c $(SRC_DIR)/$(STRING) \
		-o $(BUILD_DIR)/string.o \
		-std=gnu99 \
		-ffreestanding \
		-O2 \
		-Wall \
		-Wextra \
		-I$(SRC_DIR)/libc/include

	i686-elf-gcc -c $(SRC_DIR)/$(DISPLAY) \
		-o $(BUILD_DIR)/display.o \
		-std=gnu99 \
		-ffreestanding \
		-O2 \
		-Wall \
		-Wextra \
		-I$(SRC_DIR)/libc/include

	i686-elf-gcc -c $(SRC_DIR)/$(INPUT) \
		-o $(BUILD_DIR)/input.o \
		-std=gnu99 \
		-ffreestanding \
		-O2 \
		-Wall \
		-Wextra \
		-I$(SRC_DIR)/libc/include

	i686-elf-gcc -c $(SRC_DIR)/$(BSHELL) \
		-o $(BUILD_DIR)/bshell.o \
		-std=gnu99 \
		-ffreestanding \
		-O2 \
		-Wall \
		-Wextra \
		-I$(SRC_DIR)/libc/include

	i686-elf-gcc -c $(SRC_DIR)/$(MEMORY) \
		-o $(BUILD_DIR)/memory.o \
		-std=gnu99 \
		-ffreestanding \
		-O2 \
		-Wall \
		-Wextra \
		-I$(SRC_DIR)/libc/include

	i686-elf-gcc \
		-T $(SRC_DIR)/$(LINKER) \
		-o $(BUILD_DIR)/bsos \
		-ffreestanding \
		-O2 \
		-nostdlib \
		$(BUILD_DIR)/boot.o \
		$(BUILD_DIR)/kernel.o \
		$(BUILD_DIR)/display.o \
		$(BUILD_DIR)/string.o \
		$(BUILD_DIR)/input.o \
		$(BUILD_DIR)/bshell.o \
		$(BUILD_DIR)/memory.o \
		-lgcc

	mkdir -p $(ISO_DIR)/boot/grub

	cp $(BUILD_DIR)/bsos $(ISO_DIR)/boot/bsos
	cp $(SRC_DIR)/$(GRUBCONFIG) $(ISO_DIR)/boot/grub/grub.cfg

	grub-mkrescue -o $(BUILD_DIR)/bsos.iso $(ISO_DIR)

run:
	qemu-system-i386 \
		-cdrom $(BUILD_DIR)/bsos.iso \
		-display curses

clean:
	rm -rf $(BUILD_DIR)
