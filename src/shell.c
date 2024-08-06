
#define shell$CMD_MAXLEN 50

void shell$main(void){

    out$cstr("Welcome to ligma shell\n");
    out$cstr("\n");

	// TODO reutrn code
	// TODO history of commands

	while(true){

		out$cstr("> ");

		arr$INIT(char, cmd, shell$CMD_MAXLEN);
		in$line(cmd);

		out$cstr("you just entered: ");
		out$str(cmd);
		out$nl();

		if(arr$char$same_as$cstr(cmd, "nc")){
			out$terminal_next_visible_color();
		}else if(arr$char$same_as$cstr(cmd, "pc")){
			out$terminal_previous_visible_color();
		}else{
			out$cstr("unknown command\n");
		}

	}

}
