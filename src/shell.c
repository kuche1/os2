
#define shell$CMD_MAXLEN 50

// TODO
//
// try and debug the issue with 2 letter commands
// by compiling with gcc and adding a define that
// lets the program use the regular printf
// UPDATE: the same issue just happened for the "test" command
//
// cursor
//
// command history

#define shell$CMD_NEXT_COLOR "nec"
#define shell$CMD_PREVIOUS_COLOR "prc"
#define shell$CMD_TEST1 "test"
#define shell$CMD_TEST2 "tes"

void shell$main(void){

    out$cstr("Welcome to ligma shell\n");
    out$cstr("\n");

	err_t last_command_return_code = err$OK;

	while(true){

		out$nl();

		out$ch('[');
		out$err(last_command_return_code);

		out$cstr("] > ");

		arr$INIT(char, cmd, shell$CMD_MAXLEN);
		in$line(cmd);

		// out$cstr("you just entered: `");
		// out$str(cmd);
		// out$ch('`');
		// out$nl();

		// out$cstr("length: ");
		// out$size(cmd->len);
		// out$nl();

		if(arr$char$same_as$cstr(cmd, shell$CMD_NEXT_COLOR)){

			out$terminal_next_visible_color();

		}else if(arr$char$same_as$cstr(cmd, shell$CMD_PREVIOUS_COLOR)){

			out$terminal_previous_visible_color();

		}else if(arr$char$same_as$cstr(cmd, shell$CMD_TEST1)){

			out$cstr("test 12345\n");

		}else if(arr$char$same_as$cstr(cmd, shell$CMD_TEST2)){

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
