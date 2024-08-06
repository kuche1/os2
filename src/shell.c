
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

		out$cstr("you just entered: `");
		out$str(cmd);
		out$ch('`');
		out$nl();

		out$cstr("length: ");
		out$size(cmd->len);
		out$nl();

		if(arr$char$same_as$cstr(cmd, "nec")){
			// TODO after I enter an invalid command this refuses to get recognised
			// if I switch this and `test`, the "nc" again gets ignored
			//
			// it seems that this is a problem with all 2-length commands

			out$terminal_next_visible_color();

		}else if(arr$char$same_as$cstr(cmd, "prc")){

			out$terminal_previous_visible_color();

		}else if(arr$char$same_as$cstr(cmd, "test")){

			out$cstr("test 12345\n");

		}else if(arr$char$same_as$cstr(cmd, "tes")){

			out$cstr("ts 54543543\n");

		}else{

			out$cstr("unknown command\n");
			last_command_return_code = err$ERR;
			continue;

		}

		last_command_return_code = err$OK;

	}

}
