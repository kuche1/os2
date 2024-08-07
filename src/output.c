
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
	out$VGA_COLOR_LEN,
}out$vga_color_t;

inline uint8_t out$vga_entry_color(out$vga_color_t fg, out$vga_color_t bg){
	return (uint8_t) (fg | bg << 4);
}

inline uint16_t out$vga_entry(unsigned char uc, uint8_t color){
	return (uint16_t) ( (uint16_t) uc | (uint16_t) color << 8 );
}

const size_t out$VGA_WIDTH = 80;
const size_t out$VGA_HEIGHT = 25;

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

void out$terminal_putentryat(char ch, uint8_t color, size_t x, size_t y){
	const size_t index = y * out$VGA_WIDTH + x;
	out$terminal_buffer[index] = out$vga_entry((unsigned char) ch, color);
}

void out$terminal_next_visible_color(void){

	out$terminal_color += 1;

	if(out$terminal_color >= out$VGA_COLOR_LEN){
		out$terminal_color = 0;
	}

	if(out$terminal_color == out$VGA_COLOR_BLACK){
		out$terminal_color += 1;
	}

}

void out$terminal_previous_visible_color(void){

	if(out$terminal_color > 0){
		out$terminal_color -= 1;
	}else{
		out$terminal_color = out$VGA_COLOR_LEN - 1;
	}

	// we don't need to check for black since it's is 0

}

void out$clear_last_char(void){

	if(out$terminal_column <= 0){

		if(out$terminal_row <= 0){

			out$terminal_previous_visible_color();

			out$terminal_row = out$VGA_HEIGHT - 1;
			out$terminal_column = out$VGA_WIDTH - 1;


		}else{

			out$terminal_row -= 1;
			out$terminal_column = out$VGA_WIDTH - 1;

		}

	}else{

		out$terminal_column -= 1;

	}

	out$terminal_putentryat(' ', 0, out$terminal_column, out$terminal_row);
}

void out$nl(void){

	out$terminal_column = 0;
	out$terminal_row += 1;

	if(out$terminal_row == out$VGA_HEIGHT){
		out$terminal_row = 0;
		out$terminal_next_visible_color();
	}

	// clear the new line
	for(size_t i=0; i<out$VGA_WIDTH; ++i){
		out$terminal_putentryat(' ', 0, i, out$terminal_row);
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

// void out$str(const str_t str){
// 	size_t len = str.len;

// 	while(len--){
// 		out$ch(str.data[len]);
// 	}
// }

void out$str(arr_char_t * str){
	for(size_t i=0; i<str->len; ++i){
		out$ch(str->data[i]);
	}
}

void out$err(err_t err){
	if(err){
		out$cstr("err");
	}else{
		out$cstr("ok");
	}
}

void out$size(size_t data){
	size_t data_trimmed;
	if(data >= 10){
		data_trimmed = data % 10;
		out$size(data / 10);
	}else{
		data_trimmed = data;
	}
	char ch = '0' + (char) data_trimmed;
	out$ch(ch);
}
