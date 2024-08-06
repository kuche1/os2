
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "declaration.c"
#include "generic.c"
#include "output.c"
#include "input.c"
#include "shell.c"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler"
#endif

void kernel_main(void){

	out$terminal_initialise();

	shell$main();

	out$cstr("You can now unplug your machine");

}
