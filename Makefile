ASM = nasm
ASM_FLAGS = -f bin -I $(SRC_DIR)/boot/

# C compiler and linker setup
CC = gcc
CC_FLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -O2 -Wall
LD = ld
LD_FLAGS = -m elf_i386 -T linker.ld --oformat binary

SRC_DIR = src
BUILD_DIR = build
SCRIPT_DIR = scripts

TARGET_BIN = $(BUILD_DIR)/boot.bin
TARGET_KERNEL = $(BUILD_DIR)/kernel.bin
TARGET_ISO = $(BUILD_DIR)/os.iso

all: iso kernel

.PHONY: all iso bin clean kernel

iso: $(TARGET_ISO)
bin: $(TARGET_BIN)
kernel: $(TARGET_KERNEL)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET_BIN): $(BUILD_DIR) $(SRC_DIR)/boot/boot0.asm $(SRC_DIR)/boot/disk_read.asm $(SRC_DIR)/boot/print.asm
	$(ASM) $(ASM_FLAGS) -o $(TARGET_BIN) $(SRC_DIR)/boot/boot0.asm

$(TARGET_KERNEL): $(BUILD_DIR) $(SRC_DIR)/kernel/kernel.c
	$(CC) $(CC_FLAGS) -c $(SRC_DIR)/kernel/kernel.c -o $(BUILD_DIR)/kernel.o
	$(LD) $(LD_FLAGS) -o $(TARGET_KERNEL) $(BUILD_DIR)/kernel.o

$(TARGET_ISO): $(TARGET_BIN)
	mkdir -p $(BUILD_DIR)/iso
	cp $(TARGET_BIN) $(BUILD_DIR)/iso/os_floppy.img
	qemu-img resize -f raw $(BUILD_DIR)/iso/os_floppy.img 1440k
	xorriso -as mkisofs -V "OS" -b os_floppy.img -hide os_floppy.img -o $(TARGET_ISO) $(BUILD_DIR)/iso/

clean:
	@echo "Cleaning build artifacts..."
	- rm -rf $(BUILD_DIR)
	- rm -f boot.bin os.iso
	@echo "Clean complete."

