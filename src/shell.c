
void shell$main(void){

    out$cstr("Welcome to ligma shell\n");
    out$cstr("\n");

	// TODO reutrn code

	while(true){

		out$cstr("> ");

		arr$INIT(char, line, 90);
		in$line(line);

		out$cstr("you just entered: ");
		out$str(line);
		out$nl();

		if(arr$char$same_as$cstr(line, "nc")){
			out$terminal_next_visible_color();
		}else if(arr$char$same_as$cstr(line, "pc")){
			out$terminal_previous_visible_color();
		}else{
			out$cstr("unknown command\n");
		}

	}

}
