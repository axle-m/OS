ASM = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy

SRC_DIR = src
BUILD_DIR = build

ASM_FLAGS = -f bin -I $(SRC_DIR)/boot/

CFLAGS = \
	-ffreestanding \
	-fno-stack-protector \
	-fno-pic \
	-m32 \
	-nostdlib \
	-Wall \
	-Wextra \
	-c

LDFLAGS = \
	-m elf_i386 \
	-T $(SRC_DIR)/kernel/linker.ld

BOOT0_BIN = $(BUILD_DIR)/boot0.bin
BOOT1_BIN = $(BUILD_DIR)/boot1.bin

ENTRY_OBJ = $(BUILD_DIR)/entry.o
KERNEL_OBJ = $(BUILD_DIR)/kernel.o
KERNEL_ELF = $(BUILD_DIR)/kernel.elf
KERNEL_BIN = $(BUILD_DIR)/kernel.bin

DISK_IMAGE = $(BUILD_DIR)/os_floppy.img
ISO_IMAGE = $(BUILD_DIR)/os.iso

.PHONY: all iso clean run

all: iso

iso: $(ISO_IMAGE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# bootloader

$(BOOT0_BIN): \
	$(SRC_DIR)/boot/boot0.asm \
	$(SRC_DIR)/boot/disk_read.asm \
	$(SRC_DIR)/boot/print.asm \
	| $(BUILD_DIR)

	$(ASM) $(ASM_FLAGS) -o $@ $<

$(BOOT1_BIN): \
	$(SRC_DIR)/boot/boot1.asm \
	$(SRC_DIR)/boot/disk_read.asm \
	$(SRC_DIR)/boot/print.asm \
	| $(BUILD_DIR)

	$(ASM) $(ASM_FLAGS) -o $@ $<

# Kernel compilation

$(ENTRY_OBJ): \
	$(SRC_DIR)/kernel/entry.asm \
	| $(BUILD_DIR)

	$(ASM) -f elf32 $< -o $@

$(KERNEL_OBJ): \
	$(SRC_DIR)/kernel/kernel.c \
	| $(BUILD_DIR)

	$(CC) $(CFLAGS) $< -o $@

# Link kernel ELF

$(KERNEL_ELF): \
	$(ENTRY_OBJ) \
	$(KERNEL_OBJ) \
	$(SRC_DIR)/kernel/linker.ld

	$(LD) $(LDFLAGS) -o $@ $(ENTRY_OBJ) $(KERNEL_OBJ)

# Convert ELF -> flat binary

$(KERNEL_BIN): $(KERNEL_ELF)

	$(OBJCOPY) -O binary $< $@

# Build floppy image

$(DISK_IMAGE): \
	$(BOOT0_BIN) \
	$(BOOT1_BIN) \
	$(KERNEL_BIN)

	cat \
		$(BOOT0_BIN) \
		$(BOOT1_BIN) \
		$(KERNEL_BIN) \
		> $@

	qemu-img resize -f raw $@ 1440k

# Build ISO

$(ISO_IMAGE): $(DISK_IMAGE)

	mkdir -p $(BUILD_DIR)/iso

	cp $(DISK_IMAGE) \
	   $(BUILD_DIR)/iso/os_floppy.img

	xorriso -as mkisofs \
		-V "OS" \
		-b os_floppy.img \
		-hide os_floppy.img \
		-o $@ \
		$(BUILD_DIR)/iso/

run: $(DISK_IMAGE)

	qemu-system-x86_64 \
		-drive format=raw,file=$(DISK_IMAGE)

clean:
	@echo off
	rm -rf $(BUILD_DIR)

