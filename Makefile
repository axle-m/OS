ASM = nasm
ASM_FLAGS = -f bin -I $(SRC_DIR)/boot/
SRC_DIR = src
BUILD_DIR = build
SCRIPT_DIR = scripts
TARGET_BIN = $(BUILD_DIR)/boot.bin
TARGET_ISO = $(BUILD_DIR)/os.iso

all: iso

.PHONY: all iso bin clean

iso: $(TARGET_ISO)
bin: $(TARGET_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET_BIN): $(BUILD_DIR) $(SRC_DIR)/boot/boot0.asm $(SRC_DIR)/boot/disk_read.asm $(SRC_DIR)/boot/print.asm
	$(ASM) $(ASM_FLAGS) -o $(TARGET_BIN) $(SRC_DIR)/boot/boot0.asm

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

