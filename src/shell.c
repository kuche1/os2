
void shell$main(void){

    out$cstr("Welcome to ligma shell\n");
    out$cstr("\n");

	while(true){

		out$cstr("> ");
		arr$INIT(char, line, 20);

		in$line(line);

		out$cstr("you just entered: ");
		out$str(line);
		out$nl();

	}

}
