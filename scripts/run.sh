#!/usr/bin/env bash

set -e

ISO=false
HDD=false

while [[ $# -gt 0 ]]; do
    case "$1" in
        --iso)
            ISO=true
            ;;
        --hdd)
            HDD=true
            ;;
        *)
            echo "Unknown option: $1"
            echo "Usage: $0 [--iso] [--hdd]"
            exit 1
            ;;
    esac
    shift
done

if [[ ! -f "build/os.iso" ]]; then
    echo "Please run 'make' to compile first."
    exit 1
fi

if $ISO; then
    # Emulate an optical drive with the ISO
    qemu-system-i386 -cdrom build/os.iso

elif $HDD; then
    # Emulate booting from a hard drive
    qemu-system-i386 \
        -drive format=raw,file=build/os.iso,index=0,media=disk

else
    # Emulate booting from a floppy
    if [[ -f "build/iso/os_floppy.img" ]]; then
        qemu-system-i386 \
            -drive format=raw,file=build/iso/os_floppy.img,if=floppy
    else
        qemu-system-i386 \
            -drive format=raw,file=build/boot.bin,if=floppy
    fi
fi