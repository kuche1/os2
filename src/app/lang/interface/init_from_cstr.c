
#define lang$init_from_cstr$WORD_MAXLEN 10

#define lang$init_from_cstr$INST_MAX_ARGS 10

#include "compiler.c"

err_t lang$translate_cstr_inst_to_bytecode_inst(
    lang$compiler_t * ctx,
    char * inst, size_t inst_len,
    char arguments[lang$init_from_cstr$WORD_MAXLEN][lang$init_from_cstr$INST_MAX_ARGS], size_t * argument_lens, size_t arguments_len, // the array is passed by reference
    bool * inst0_set, uint8_t * inst0, uint8_t * inst0_arg,
    bool * inst1_set, uint8_t * inst1, uint8_t * inst1_arg
){

    * inst0_set = false;
    * inst1_set = false;

    // get data or convert variable to data

    // uint8_t arg_u8;
    // {
    //     err_t err = strlen_to_u8(arg, arg_len, & arg_u8);

    //     if(err){

    //         err_t err_find = lang$compiler_t$find_var(ctx, arg, arg_len, & arg_u8);

    //         if(err_find){
    //             out$cstr("could not find variable `");
    //             out$strlen(arg, arg_len);
    //             out$cstr("`\n");
    //             return err$err;
    //         }

    //     }

    // }

    // uint8_t arg_u8;
    // {
    //     err_t err = lang$compiler_t$get_arg_value(ctx, arg, arg_len, &arg_u8);
    //     if(err){
    //         return err;
    //     }
    // }

    // set variable value
    // note that this is going to fuck you over if some
    // retard has overwritten certain addresses as variable names

    do{
        uint8_t var_addr;
        {
            err_t err = lang$compiler_t$find_var(ctx, inst, inst_len, &var_addr);
            if(err){
                break;
            }
        }

        if(arguments_len != 1){
            out$cstr("bad number of arguments\n");
            return err$err;
        }

        uint8_t arg_value;
        err_t err = lang$compiler_t$get_arg_value(ctx, arguments[0], argument_lens[0], &arg_value);
        if(err){
            return err;
        }
  
        * inst0_set = true;
        * inst0 = lang$ic$copy$arg$0x00;
        * inst0_arg = arg_value;

        * inst1_set = true;
        * inst1 = lang$ic$copy$0x00$cell;
        * inst1_arg = var_addr;

        return err$ok;

    }while(false);
    
    // actual instruction

    if(arguments_len != 1){
        out$cstr("bad number of arguments\n");
        return err$err;
    }

    uint8_t arg_value;
    err_t err = lang$compiler_t$get_arg_value(ctx, arguments[0], argument_lens[0], &arg_value);
    if(err){
        return err;
    }

    * inst0_arg = arg_value;

    if(strlen_sameas_cstr(inst, inst_len, "out$arg")){
        * inst0_set = true;
        * inst0 = lang$ic$out$arg;
        return err$ok;
    }else if(strlen_sameas_cstr(inst, inst_len, "out$cell")){
        * inst0_set = true;
        * inst0 = lang$ic$out$cell;
        return err$ok;
    }

    // error

    out$cstr("unknown instruction `");
    out$strlen(inst, inst_len);
    out$cstr("`\n");

    return err$err;
}

err_t lang$program_data_t$init_from_cstr(lang$program_data_t * ctx, char * cstr_code, uint8_t * ic_code, size_t ic_code_cap){
    
    lang$compiler_t compiler;
    lang$compiler_t$init(&compiler);

    size_t ic_code_len = 0;

    size_t cstr_code_idx = 0;

    char word[lang$init_from_cstr$WORD_MAXLEN];
    size_t word_len = 0;

    char inst[LENOF(word)];
    size_t inst_len = 0;

    char arguments[LENOF(word)][lang$init_from_cstr$INST_MAX_ARGS];
    size_t argument_lens[lang$init_from_cstr$INST_MAX_ARGS];
    size_t arguments_len = 0;
    COMPTIME_ASSERT(LENOF(arguments) == lang$init_from_cstr$INST_MAX_ARGS); // TODO delete

    while(true){

        char ch = cstr_code[cstr_code_idx++];
        if(ch == 0){
            break;
        }

        if((ch == ' ') || (ch == '\n')){

            if(word_len <= 0){
                continue;
            }

            if(inst_len == 0){
                err$CHECK( copy(word, word_len, inst, sizeof(inst), word_len) , "copy fail\n");
                inst_len = word_len;
                word_len = 0;
                continue;
            }

            if(arguments_len >= LENOF(arguments)){
                out$cstr("too many arguments\n");
                return err$err;
            }

            err$CHECK( copy(word, word_len, arguments[arguments_len], sizeof(arguments[arguments_len]), word_len) , "copy fail\n");
            argument_lens[arguments_len] = word_len;

            arguments_len += 1;
            word_len = 0;

            if(ch != '\n'){
                continue;
            }

            bool is_compiler_directive;
            err_t pcd_err = lang$compiler_t$process_directive(
                &compiler,
                inst, inst_len,
                arguments, argument_lens, arguments_len,
                &is_compiler_directive
            );

            if(pcd_err){
                return pcd_err;
            }

            if(!is_compiler_directive){

                bool inst0_set = false;
                uint8_t inst0;
                uint8_t inst0_arg;

                bool inst1_set = false;
                uint8_t inst1;
                uint8_t inst1_arg;

                err_t err = lang$translate_cstr_inst_to_bytecode_inst(
                    &compiler,
                    inst, inst_len,
                    arguments, argument_lens, arguments_len,
                    &inst0_set, &inst0, &inst0_arg,
                    &inst1_set, &inst1, &inst1_arg
                );

                if(err){
                    return err$err;
                }

                if(inst0_set){

                    if(ic_code_len >= ic_code_cap){
                        out$cstr("bytecode capacity reached (instruction)\n");
                        return err$err;
                    }

                    ic_code[ic_code_len++] = inst0;

                    if(ic_code_len >= ic_code_cap){
                        out$cstr("bytecode capacity reached (argument)\n");
                        return err$err;
                    }

                    ic_code[ic_code_len++] = inst0_arg;

                }

                if(inst1_set){

                    if(ic_code_len >= ic_code_cap){
                        out$cstr("bytecode capacity reached (instruction)\n");
                        return err$err;
                    }

                    ic_code[ic_code_len++] = inst1;

                    if(ic_code_len >= ic_code_cap){
                        out$cstr("bytecode capacity reached (argument)\n");
                        return err$err;
                    }

                    ic_code[ic_code_len++] = inst1_arg;

                }

            }

            inst_len = 0;
            arguments_len = 0;

        }else{

            if(word_len >= LENOF(word)){
                out$cstr("keyword is too long\n");
                return err$err;
            }

            word[word_len++] = ch;

        }

    }

    return lang$program_data_t$init_from_instruction_code(ctx, ic_code, ic_code_len);

}
