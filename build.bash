#!/bin/bash

set -e
clear

CCFLAGS="-std=gnu99 -ffreestanding -O2 -Wall -Wextra"
ASFLAGS="-felf32"
LDFLAGS="-ffreestanding -O2 -nostdlib -lgcc"
GCC=i686-elf-tools/output/bin/i686-elf-gcc
NASM=nasm
GRUB_MKRESCUE=grub-mkrescue

# clean up old build
bash clean.bash
# build kernel
mkdir build
# compile all the c files
for f in $(find ./kernel -name '*.c'); do
    base=$(basename $f)
    $GCC -c $f -o build/${base}.o $CCFLAGS
done;
# compile all the assembly files
for f in $(find ./kernel -name '*.s'); do
    base=$(basename $f)
    $NASM $f -o build/${base}.o $ASFLAGS
done;
# linking it all together
$GCC -T linker.ld -o build/os.bin build/*.o $LDFLAGS

# make iso
mkdir -p isodir/boot/grub
cp build/os.bin isodir/boot/os.bin
echo "menuentry \"os\" {
	multiboot /boot/os.bin
}" > isodir/boot/grub/grub.cfg
$GRUB_MKRESCUE -o os.iso isodir