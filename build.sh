#! /usr/bin/env bash

# info
#
# https://wiki.osdev.org/Bare_Bones

set -euo pipefail

HERE=$(dirname "$BASH_SOURCE")

i686-elf-as "$HERE/boot.s" -o "$HERE/boot.o" # paru i686-elf-gcc

i686-elf-gcc -c "$HERE/kernel.c" -o "$HERE/kernel.o" -std=gnu99 -ffreestanding -O2 -Wall -Wextra

i686-elf-gcc -T "$HERE/linker.ld" -o "$HERE/myos.bin" -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

grub-file --is-x86-multiboot "$HERE/myos.bin" # sudo pacman -S grub

rm -rf "$HERE/isodir"
mkdir -p "$HERE/isodir/boot/grub"
cp "$HERE/myos.bin" "$HERE/isodir/boot/myos.bin"
cp "$HERE/grub.cfg" "$HERE/isodir/boot/grub/grub.cfg"
grub-mkrescue -o "$HERE/myos.iso" "$HERE/isodir" # sudo pacman -S libisoburn

# qemu-system-i386 -cdrom myos.iso
