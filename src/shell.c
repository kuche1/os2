
void shell$main(void){

    out$cstr("Welcome to ligma shell\n");
    out$cstr("\n");

	// while(true){

	// 	out$cstr("> ");
	// 	str_t cmd = in$str();

	// 	char entered = in$ch();

	// 	out$cstr("You just entered: ");
	// 	out$ch(entered);
	// 	out$ch('\n');

	// }

	// arr$INIT(char, line, 40);

	// arr$push_char(line, 's');

	while(true){

		out$cstr("> ");
		arr$INIT(char, line, 40);

		in$line(line);

		out$cstr("you just entered: ");
		out$str(line);
		out$nl();

	}

}
