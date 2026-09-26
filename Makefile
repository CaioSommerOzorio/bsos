BOOT = boot.s
KERNEL = kernel.c
LINKER = linker.ld
GRUBCONFIG = grub.cfg
SRC_DIR = src
BUILD_DIR = build
ISO_DIR = build/isodir/

.PHONY: build

build:
	mkdir -p $(BUILD_DIR)
	i686-elf-as $(SRC_DIR)/$(BOOT) -o $(BUILD_DIR)/boot.o
	i686-elf-gcc -c $(SRC_DIR)/$(KERNEL) -o $(BUILD_DIR)/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	i686-elf-gcc -T $(SRC_DIR)/$(LINKER) -o $(BUILD_DIR)/bsos -ffreestanding -O2 -nostdlib $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o -lgcc
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(BUILD_DIR)/bsos $(ISO_DIR)/boot/bsos
	cp $(SRC_DIR)/$(GRUBCONFIG) $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(BUILD_DIR)/bsos.iso $(ISO_DIR)

run:
	qemu-system-i386 -cdrom $(BUILD_DIR)/bsos.iso

clean:
	rm -rf $(BUILD_DIR)
