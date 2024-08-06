
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "generic.c"
#include "output.c"
#include "input.c"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler"
#endif

void kernel_main(void){

	terminal_initialize();

	print_cstr("Hello, kernel World!\n");
	print_cstr("asdfg\n");

	while(true){

		print_cstr("Enter some input: \n");

		char entered = getch();

		print_cstr("You just entered: ");
		print_ch(entered);
		print_ch('\n');

	}

}
