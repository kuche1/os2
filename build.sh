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

FOLDER_RELEASE="$HERE/release"
FOLDER_ISO="$FOLDER_RELEASE/iso"

rm -rf "$FOLDER_ISO"
mkdir -p "$FOLDER_ISO/boot/grub"
cp "$HERE/myos.bin" "$FOLDER_ISO/boot/myos.bin"
cp "$HERE/grub.cfg" "$FOLDER_ISO/boot/grub/grub.cfg"
grub-mkrescue -o "$FOLDER_RELEASE/myos.iso" "$FOLDER_ISO" # sudo pacman -S libisoburn

qemu-system-i386 -cdrom myos.iso
