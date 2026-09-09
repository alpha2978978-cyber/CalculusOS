# ============================================================
# CalculusOS build system
#
# Requires: gcc + binutils able to target 32-bit x86 (most Linux
# distros' regular gcc can do this with -m32; an i686-elf cross
# compiler works too and is the "textbook" OSDev way - either is
# fine here). To produce a bootable .iso you additionally need
# grub-mkrescue + xorriso.
# ============================================================

CC      = gcc
AS      = gcc
LD      = ld

CFLAGS  = -m32 -ffreestanding -fno-pie -fno-stack-protector \
          -fno-builtin -nostdlib -Wall -Wextra -O2 -std=gnu11
ASFLAGS = -m32 -ffreestanding -c
LDFLAGS = -m elf_i386 -T src/linker.ld -nostdlib

SRC_DIR   = src
BUILD_DIR = build
ISO_DIR   = iso

C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
S_SOURCES = $(wildcard $(SRC_DIR)/*.s)
OBJECTS   = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES)) \
            $(patsubst $(SRC_DIR)/%.s,$(BUILD_DIR)/%.o,$(S_SOURCES))

KERNEL    = $(BUILD_DIR)/calculusos.bin
ISO       = $(BUILD_DIR)/CalculusOS.iso

.PHONY: all clean iso run

all: $(KERNEL)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(KERNEL): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

# Bootable ISO (needs grub-mkrescue + xorriso installed)
iso: $(KERNEL)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL) $(ISO_DIR)/boot/calculusos.bin
	grub-mkrescue -o $(ISO) $(ISO_DIR)

run: iso
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf $(BUILD_DIR)
