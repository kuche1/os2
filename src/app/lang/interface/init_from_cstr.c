
// TODO all of this is untested

#define lang$program_data_t$init_from_cstr$WORD_MAXLEN 10
// TODO increase after it has been tested

err_or_u8_u8_t lang$program_data_t$init_from_cstr$translate_cstr_inst_to_bytecode_inst(char * inst, size_t inst_len, char * arg, size_t arg_len){

    uint8_t arg_u8;
    {
        err_or_u8_t eor = strlen_to_u8(arg, arg_len);
        if(eor.err){
            out$cstr("could not convert instruction argument to u8 `");
            out$cstr_len(arg, arg_len);
            out$cstr("`\n");
        }

        arg_u8 = eor.data;
    }

    if(strlen_sameas_cstr(inst, inst_len, "out$arg")){
        return (err_or_u8_u8_t) {.err=err$OK, .data0=lang$ic$out$arg, .data1=arg_u8};
    }

    out$cstr("unknown instruction `");
    out$cstr_len(inst, inst_len);
    out$cstr("`\n");

    return (err_or_u8_u8_t) {.err=err$ERR, .data0=0, .data1=0};
}

err_t lang$program_data_t$init_from_cstr(lang$program_data_t * ctx, char * cstr_code, uint8_t * ic_code, size_t ic_code_cap){
    
    size_t ic_code_len = 0;

    size_t cstr_code_idx = 0;

    char word[lang$program_data_t$init_from_cstr$WORD_MAXLEN];
    size_t word_len = 0;

    char inst[LENOF(word)];
    size_t inst_len = 0;

    char arg[LENOF(word)];
    size_t arg_len = 0;

    while(true){

        char ch = cstr_code[cstr_code_idx++];
        if(ch == 0){
            break;
        }

        if(ch == ' '){

            if(inst_len != 0){
                out$cstr("instruction already specified\n");
                return err$ERR;
            }

            copy(word, inst, word_len);
            inst_len = word_len;

            word_len = 0;

        }else if(ch == '\n'){

            if(arg_len != 0){
                out$cstr("argument already specified\n");
                return err$ERR;
            }

            copy(word, arg, word_len);
            arg_len = word_len;

            word_len = 0;

            UNP_U8_U8(err, data0, data1, lang$program_data_t$init_from_cstr$translate_cstr_inst_to_bytecode_inst(inst, inst_len, arg, arg_len));

            if(err){
                return err$ERR;
            }

            if(ic_code_len >= ic_code_cap){
                out$cstr("bytecode capacity reached (instruction)\n");
                return err$ERR;
            }

            ic_code[ic_code_len++] = data0;

            if(ic_code_len >= ic_code_cap){
                out$cstr("bytecode capacity reached (argument)\n");
                return err$ERR;
            }

            ic_code[ic_code_len++] = data1;

            inst_len = 0;
            arg_len = 0;

        }else{

            if(word_len >= LENOF(word)){
                out$cstr("keyword is too long\n");
                return err$ERR;
            }

            word[word_len++] = ch;

        }

    }

    return lang$program_data_t$init_from_instruction_code(ctx, ic_code, ic_code_len);

}
