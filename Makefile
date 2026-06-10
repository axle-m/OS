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
	-c \
	-MMD \
	-MP

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

# -------------------------------------------------
# Recursive libk source discovery
# -------------------------------------------------

LIBK_DIR := $(SRC_DIR)/libk

LIBK_SRCS := $(shell find $(LIBK_DIR) -name '*.c')

# Mirror directory structure into build/
LIBK_OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(LIBK_SRCS))

# -------------------------------------------------
# Recursive shell source discovery
# -------------------------------------------------
SHELL_DIR := $(SRC_DIR)/shell
SHELL_SRCS := $(shell find $(SHELL_DIR) -name '*.c')
SHELL_OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SHELL_SRCS))


# -------------------------------------------------
# Phony targets
# -------------------------------------------------

.PHONY: all iso clean run

all: iso

iso: $(ISO_IMAGE)

# -------------------------------------------------
# Build directory
# -------------------------------------------------

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# -------------------------------------------------
# Bootloader
# -------------------------------------------------

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

# -------------------------------------------------
# Kernel objects
# -------------------------------------------------

$(ENTRY_OBJ): \
	$(SRC_DIR)/kernel/entry.asm \
	| $(BUILD_DIR)

	mkdir -p $(dir $@)
	$(ASM) -f elf32 $< -o $@

$(KERNEL_OBJ): \
	$(SRC_DIR)/kernel/kernel.c \
	| $(BUILD_DIR)

	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

# -------------------------------------------------
# Generic rule for ALL C files (kernel + libk)
# -------------------------------------------------

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# -------------------------------------------------
# Link kernel ELF
# -------------------------------------------------

$(KERNEL_ELF): \
	$(ENTRY_OBJ) \
	$(KERNEL_OBJ) \
	$(LIBK_OBJS) \
	$(SHELL_OBJS) \
	$(SRC_DIR)/kernel/linker.ld

	$(LD) $(LDFLAGS) -o $@ \
		$(ENTRY_OBJ) \
		$(KERNEL_OBJ) \
		$(LIBK_OBJS) \
		$(SHELL_OBJS)

# -------------------------------------------------
# ELF → flat binary
# -------------------------------------------------

$(KERNEL_BIN): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $< $@

# -------------------------------------------------
# Disk image
# -------------------------------------------------

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

# -------------------------------------------------
# ISO image
# -------------------------------------------------

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

# -------------------------------------------------
# Run
# -------------------------------------------------

run: $(DISK_IMAGE)
	qemu-system-x86_64 \
		-drive format=raw,file=$(DISK_IMAGE)

# -------------------------------------------------
# Clean
# -------------------------------------------------

clean:
	rm -rf $(BUILD_DIR)

# -------------------------------------------------
# Dependency tracking
# -------------------------------------------------

-include $(BUILD_DIR)/*.d