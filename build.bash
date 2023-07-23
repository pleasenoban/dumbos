#!/bin/bash

set -e
clear

GCC=i686-elf-tools/output/bin/i686-elf-gcc
NASM=nasm
GRUB_MKRESCUE=grub-mkrescue

# clean up old build
bash clean.bash
# build kernel
mkdir build
# compile all the c files
# $GCC -c kernel/kernel.c -o build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
for f in $(find ./kernel -name '*.c'); do
    base=$(basename $f)
    $GCC -c $f -o build/${base}.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
done;
# compile all the assembly files
# $NASM -felf32 kernel/boot.s -o build/boot.o
for f in $(find ./kernel -name '*.s'); do
    base=$(basename $f)
    $NASM -felf32 $f -o build/${base}.o
done;
# linking it all together
$GCC -T linker.ld -o build/os.bin -ffreestanding -O2 -nostdlib build/*.o -lgcc

# make iso
mkdir -p isodir/boot/grub
cp build/os.bin isodir/boot/os.bin
echo "menuentry \"os\" {
	multiboot /boot/os.bin
}" > isodir/boot/grub/grub.cfg
$GRUB_MKRESCUE -o os.iso isodir