#! /usr/bin/env bash

# info
#
# https://wiki.osdev.org/Bare_Bones

set -euo pipefail

HERE=$(dirname "$BASH_SOURCE")
FOLDER_SRC="$HERE/src"
FOLDER_BOOT="$HERE/boot"
FOLDER_RELEASE="$HERE/release"
FOLDER_ISO="$FOLDER_RELEASE/iso"

i686-elf-as "$FOLDER_BOOT/boot.s" -o "$FOLDER_RELEASE/boot.o" # paru i686-elf-gcc

i686-elf-gcc -c "$FOLDER_SRC/kernel.c" -o "$FOLDER_RELEASE/kernel.o" -std=gnu99 -ffreestanding -O2 -Wall -Wextra

i686-elf-gcc -T "$FOLDER_BOOT/linker.ld" -o "$FOLDER_RELEASE/myos.bin" -ffreestanding -O2 -nostdlib "$FOLDER_RELEASE/boot.o" "$FOLDER_RELEASE/kernel.o" -lgcc

grub-file --is-x86-multiboot "$FOLDER_RELEASE/myos.bin" # sudo pacman -S grub

rm -rf "$FOLDER_ISO"
mkdir -p "$FOLDER_ISO/boot/grub"
cp "$FOLDER_RELEASE/myos.bin" "$FOLDER_ISO/boot/myos.bin"
cp "$FOLDER_BOOT/grub.cfg" "$FOLDER_ISO/boot/grub/grub.cfg"
grub-mkrescue -o "$FOLDER_RELEASE/myos.iso" "$FOLDER_ISO" # sudo pacman -S libisoburn

qemu-system-i386 -cdrom "$FOLDER_RELEASE/myos.iso"
