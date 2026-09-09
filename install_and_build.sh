#!/usr/bin/env bash
# ============================================================
# CalculusOS - install_and_build.sh
# Installs everything needed (Debian/Ubuntu) and builds the
# kernel binary + bootable ISO in one go.
#
# Usage:
#   chmod +x install_and_build.sh
#   ./install_and_build.sh
# ============================================================
set -e

echo "==> Installing build tools (gcc, binutils, make, grub, xorriso, qemu)..."
sudo apt update
sudo apt install -y \
    build-essential \
    grub-pc-bin \
    grub-common \
    xorriso \
    qemu-system-x86 \
    mtools

echo "==> Cleaning old build artifacts..."
make clean

echo "==> Building kernel (build/calculusos.bin)..."
make

echo "==> Building bootable ISO (build/CalculusOS.iso)..."
make iso

echo ""
echo "==> Done. Kernel:  build/calculusos.bin"
echo "==> Done. ISO:     build/CalculusOS.iso"
echo ""
echo "Run it now with:"
echo "  qemu-system-i386 -cdrom build/CalculusOS.iso"
echo ""
read -p "Launch it in QEMU now? [y/N] " ans
if [[ "$ans" == "y" || "$ans" == "Y" ]]; then
    qemu-system-i386 -cdrom build/CalculusOS.iso
fi
