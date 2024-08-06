
/* Hardware text mode color constants. */
typedef enum{
	out$VGA_COLOR_BLACK = 0,
	out$VGA_COLOR_BLUE = 1,
	out$VGA_COLOR_GREEN = 2,
	out$VGA_COLOR_CYAN = 3,
	out$VGA_COLOR_RED = 4,
	out$VGA_COLOR_MAGENTA = 5,
	out$VGA_COLOR_BROWN = 6,
	out$VGA_COLOR_LIGHT_GREY = 7,
	out$VGA_COLOR_DARK_GREY = 8,
	out$VGA_COLOR_LIGHT_BLUE = 9,
	out$VGA_COLOR_LIGHT_GREEN = 10,
	out$VGA_COLOR_LIGHT_CYAN = 11,
	out$VGA_COLOR_LIGHT_RED = 12,
	out$VGA_COLOR_LIGHT_MAGENTA = 13,
	out$VGA_COLOR_LIGHT_BROWN = 14,
	out$VGA_COLOR_WHITE = 15,
}out$vag_color_t;

static inline uint8_t out$vga_entry_color(out$vag_color_t fg, out$vag_color_t bg){
	return fg | bg << 4;
}

static inline uint16_t out$vga_entry(unsigned char uc, uint8_t color){
	return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t out$VGA_WIDTH = 80;
static const size_t out$VGA_HEIGHT = 25;

size_t out$terminal_row;
size_t out$terminal_column;
uint8_t out$terminal_color;
uint16_t* out$terminal_buffer;

void out$terminal_initialise(void){
	out$terminal_row = 0;
	out$terminal_column = 0;
	out$terminal_color = out$vga_entry_color(out$VGA_COLOR_LIGHT_GREY, out$VGA_COLOR_BLACK);
	out$terminal_buffer = (uint16_t*) 0xB8000;
	for(size_t y = 0; y < out$VGA_HEIGHT; y++){
		for(size_t x = 0; x < out$VGA_WIDTH; x++){
			const size_t index = y * out$VGA_WIDTH + x;
			out$terminal_buffer[index] = out$vga_entry(' ', out$terminal_color);
		}
	}
}

void out$terminal_setcolor(uint8_t color){
	out$terminal_color = color;
}

void out$terminal_putentryat(char c, uint8_t color, size_t x, size_t y){
	const size_t index = y * out$VGA_WIDTH + x;
	out$terminal_buffer[index] = out$vga_entry(c, color);
}

void out$nl(void){
	out$terminal_column = 0;
	out$terminal_row += 1;
	if(out$terminal_row == out$VGA_HEIGHT){
		out$terminal_row = 0;
	}
}

void out$ch(char ch){

	if(ch == '\n'){

		out$nl();

	}else{

		out$terminal_putentryat(ch, out$terminal_color, out$terminal_column, out$terminal_row);

		out$terminal_column += 1;
		if(out$terminal_column == out$VGA_WIDTH){
			out$nl();
		}

	}

}

void out$cstr(const char * data){
	char ch;
	while((ch = *data++)){
		out$ch(ch);
	}
}

void out$str(const str_t str){
	size_t len = str.len;

	while(len--){
		out$ch(str.data[len]);
	}
}
