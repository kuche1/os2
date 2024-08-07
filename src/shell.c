
#define shell$MAX_NUMBER_OF_COMMANDS 10
#define shell$CMD_MAXLEN 50

// TODO
//
// cursor
//
// command history

#define shell$CMD_NEXT_COLOR "nc"
#define shell$CMD_PREVIOUS_COLOR "pc"
#define shell$CMD_TEST1 "test"
#define shell$CMD_TEST2 "tes"

void shell$main(void){

	// arr$INIT(arr_char_t, cmds, shell$MAX_NUMBER_OF_COMMANDS);
	// arr$arr_char_t$push

	err_t last_command_return_code = err$OK;

    out$cstr("Welcome to ligma shell\n");
    out$cstr("\n");

	while(true){

		out$nl();

		out$ch('[');
		out$err(last_command_return_code);

		out$cstr("] > ");

		arr$INIT(char_t, cmd, shell$CMD_MAXLEN);
		in$line(cmd);

		// out$cstr("you just entered: `");
		// out$str(cmd);
		// out$ch('`');
		// out$nl();

		// out$cstr("length: ");
		// out$size(cmd->len);
		// out$nl();

		if(arr$char_t$same_as$cstr(cmd, shell$CMD_NEXT_COLOR)){

			out$terminal_next_visible_color();

		}else if(arr$char_t$same_as$cstr(cmd, shell$CMD_PREVIOUS_COLOR)){

			out$terminal_previous_visible_color();

		}else if(arr$char_t$same_as$cstr(cmd, shell$CMD_TEST1)){

			out$cstr("test 12345\n");

		}else if(arr$char_t$same_as$cstr(cmd, shell$CMD_TEST2)){

			out$cstr("ts 54543543\n");

		}else{

			last_command_return_code = err$ERR;

			out$cstr("unknown command: `");
			out$str(cmd);
			out$cstr("`\n");

			out$cstr("list of commands:\n");
			out$cstr(shell$CMD_NEXT_COLOR);
			out$nl();
			out$cstr(shell$CMD_PREVIOUS_COLOR);
			out$nl();
			out$cstr(shell$CMD_TEST1);
			out$nl();
			out$cstr(shell$CMD_TEST2);
			out$nl();
			
			continue;

		}

		last_command_return_code = err$OK;

	}

}
