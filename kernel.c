
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */
enum vga_color{
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg){
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color){
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str){
	size_t len = 0;
	while(str[len]){
		len++;
	}
	return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void){
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for(size_t y = 0; y < VGA_HEIGHT; y++){
		for(size_t x = 0; x < VGA_WIDTH; x++){
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color){
	terminal_color = color;
}

void terminal_putnewline(void){
	terminal_column = 0;
	terminal_row += 1;
	if(terminal_row == VGA_HEIGHT){
		terminal_row = 0;
	}
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y){
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c){

	if(c == '\n'){

		terminal_putnewline();

	}else{

		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

		terminal_column += 1;
		if(terminal_column == VGA_WIDTH){
			terminal_putnewline();
		}

	}

}

void terminal_write(const char* data, size_t size){
	for(size_t i = 0; i < size; i++){
		terminal_putchar(data[i]);
	}
}

void terminal_writestring(const char* data){
	terminal_write(data, strlen(data));
}

// I think this will work, but only in realtime mode
// https://wiki.osdev.org/BIOS 0=blocking 1=nonoblocking
// static inline char getch (void){

//    uint16_t inchar;

//    __asm__ __volatile__(
// 		"int $0x16\n\t"
// 		: "=a"(inchar)
// 		: "0"(0x0)
// 	);

//    return ((char)inchar);
// }

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_STATUS_MASK_OUT_BUF 0x01

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static char scancode_to_ascii[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', /* Backspace */
    '\t', /* Tab */
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0, /* Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, /* Right shift */
    '*',
    0,  /* Alt */
    ' ', /* Space bar */
    0,  /* Caps lock */
    0,  /* F1 key ... > */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* < ... F10 */
    0,  /* Num lock */
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',
    0,  /* End key */
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0, 0, 0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

static inline char getch(void) {
    uint8_t status;
    char keycode;

    // Wait for a key press
    while(true){
        status = inb(KEYBOARD_STATUS_PORT);
        if(status & KEYBOARD_STATUS_MASK_OUT_BUF){
            keycode = inb(KEYBOARD_DATA_PORT);
            if(keycode < sizeof(scancode_to_ascii)){
                return scancode_to_ascii[keycode];
            }
        }
    }
}

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
