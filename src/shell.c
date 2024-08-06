
void shell$main(void){

    out$cstr("Welcome to ligma shell\n");
    out$cstr("\n");

	while(true){

		out$cstr("Enter some input: \n");

		char entered = in$ch();

		out$cstr("You just entered: ");
		out$ch(entered);
		out$ch('\n');

	}

}
