
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

	terminal_writestring("Hello, kernel World!\n");
	terminal_writestring("asdfg\n");

	while(true){

		terminal_writestring("Enter some input: \n");

		char entered = getch();

		terminal_writestring("You just entered: ");
		terminal_putchar(entered);
		terminal_putchar('\n');

	}

}
