
#include "app/calculator.c"
#include "app/lang/lang.c"

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
#define shell$CMD_CALCULATOR "calc"
#define shell$CMD_LANG "lang"
#define shell$CMD_EXIT "exit"
#define shell$CMD_SSH "ssh"

void shell$main(void){

	// arr$INIT(arr_char_t, cmds, shell$MAX_NUMBER_OF_COMMANDS);
	// arr$arr_char_t$push$CSTR(cmds, shell$CMD_NEXT_COLOR);

	err_t last_command_return_code = err$ok;

    out$cstr("Welcome to ligma shell\n");
    out$cstr("\n");

	while(true){

		out$nl();

		out$ch('[');
		out$err(last_command_return_code);

		out$cstr("] > ");

		arr$INIT(char_t, cmd, shell$CMD_MAXLEN);
		in$line(cmd);

		last_command_return_code = err$ok;

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

		}else if(arr$char_t$same_as$cstr(cmd, shell$CMD_CALCULATOR)){

			last_command_return_code = calc$main();

		}else if(arr$char_t$same_as$cstr(cmd, shell$CMD_LANG)){

			last_command_return_code = lang$main();

		}else if(arr$char_t$same_as$cstr(cmd, shell$CMD_SSH)){

			out$cstr("no internet connection\n");
			last_command_return_code = err$err;

		}else if(arr$char_t$same_as$cstr(cmd, shell$CMD_EXIT)){

			break;

		}else{

			last_command_return_code = err$err;

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
			out$cstr(shell$CMD_CALCULATOR);
			out$nl();
			out$cstr(shell$CMD_LANG);
			out$nl();
			out$cstr(shell$CMD_EXIT);
			out$nl();
			out$cstr(shell$CMD_SSH);
			out$nl();

		}

	}

}
