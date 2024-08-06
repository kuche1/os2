
#define shell$CMD_MAXLEN 50

void shell$main(void){

    out$cstr("Welcome to ligma shell\n");
    out$cstr("\n");

	// TODO reutrn code
	// TODO history of commands

	err_t last_command_return_code = err$OK;

	while(true){

		out$ch('[');
		out$err(last_command_return_code);

		out$cstr("] > ");

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
			last_command_return_code = err$ERR;
			continue;
		}

		last_command_return_code = err$OK;

	}

}
