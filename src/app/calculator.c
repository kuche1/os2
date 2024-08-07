
#define calc$EQUASION_MAXLEN 100

err_t calc$main(void){

    typedef enum{
        OP_ADD,
        OP_SUB,
        OP_MUL,
        OP_DIV,
    }operator_t;

get_new_input:
    ;

    out$cstr("Enter equasion: ");

    arr$INIT(char_t, equ, calc$EQUASION_MAXLEN);
    in$line(equ);

    int a = 0;
    operator_t op = OP_ADD;
    int b = 0;

    int * arg = &a;

    while(equ->len > 0){

        UNP(empty, char, head, arr$char_t$pop_head(equ));

        if(empty){
            return err$ERR;
        }

        if(head == ' '){
            continue;
        }

        if((head >= '0') && (head <= '9')){
            *arg *= 10;
            *arg += head - '0';
            continue;
        }

        bool is_operator = false;

        if(head == '+'){
            op = OP_ADD;
            is_operator = true;
        }else if(head == '-'){
            op = OP_SUB;
            is_operator = true;
        }else if(head == '*'){
            op = OP_MUL;
            is_operator = true;
        }else if(head == '/'){
            op = OP_DIV;
            is_operator = true;
        }

        if(is_operator){
            if(arg == &a){
                arg = &b;
            }else{
                out$cstr("you can only use 1 operator at a time");
                goto get_new_input;
            }
            continue;
        }

        out$cstr("unknown operator: `");
        out$ch(head);
        out$cstr("`\n");
        goto get_new_input;

    }

    int result = a;

    switch(op){
        case OP_ADD:
            result += b;
            break;
        case OP_SUB:
            result -= b;
            break;
        case OP_MUL:
            result *= b;
            break;
        case OP_DIV:
            result /= b;
            break;
    }

    out$cstr("result: ");
    out$int(result);
    out$nl();

    return err$OK;

}
