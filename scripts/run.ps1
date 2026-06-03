param (
    [switch]$iso,
    [switch]$hdd
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path "build/os.iso")) {
    Write-Host "Please run 'make' to compile first."
    exit 1
}

if ($iso) {
    # Emulate an optical drive with the ISO
    qemu-system-i386 -cdrom build/os.iso
} elseif ($hdd) {
    # Emulate booting from a hard drive
    qemu-system-i386 -drive format=raw,file=build/os.iso,index=0,media=disk
} else {
    # Emulate booting from a standard floppy (the extracted image inside the ISO build dir)
    if (Test-Path "build/iso/os_floppy.img") {
        qemu-system-i386 -drive format=raw,file=build/iso/os_floppy.img,if=floppy
    } else {
        qemu-system-i386 -drive format=raw,file=build/boot.bin,if=floppy
    }
}
