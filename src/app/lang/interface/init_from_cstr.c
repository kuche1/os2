
#define lang$init_from_cstr$WORD_MAXLEN 10
// TODO increase after it has been tested

#include "compiler.c"

err_t lang$translate_cstr_inst_to_bytecode_inst(
    lang$compiler_t * ctx, char * inst, size_t inst_len, char * arg, size_t arg_len,
    bool * inst0_set, uint8_t * inst0, uint8_t * inst0_arg,
    bool * inst1_set, uint8_t * inst1, uint8_t * inst1_arg
){

    * inst0_set = false;
    * inst1_set = false;

    // get data or convert variable to data

    uint8_t arg_u8;
    {
        err_or_u8_t eor = strlen_to_u8(arg, arg_len);

        if(eor.err){

            err_or_u8_t var_eor = lang$compiler_t$find_var(ctx, arg, arg_len);

            if(var_eor.err){
                out$cstr("could not find variable `");
                out$strlen(arg, arg_len);
                out$cstr("`\n");
                return err$ERR;
            }else{
                arg_u8 = var_eor.data;
                // out$cstr("[dbg: found variable `");
                // out$strlen(arg, arg_len);
                // out$cstr("` and converted to number `");
                // out$u8(arg_u8);
                // out$cstr("`]");
            }

        }else{
            
            arg_u8 = eor.data;

        }
    }

    // variable
    // note that this is going to fuck you over
    // if some retard has overwritten certain addresses as variable names

    {
        err_or_u8_t eod = lang$compiler_t$find_var(ctx, inst, inst_len);

        if(!eod.err){
            
            // out$cstr("[dbg: converted fake instruction `");
            // out$strlen(inst, inst_len);
            // out$cstr("` into address `");
            // out$u8(eod.data);
            // out$cstr("`]");

            // cell with name "inst" needs to get the data from cell located at "arg_u8"

            * inst0_set = true;
            * inst0 = lang$ic$copy$arg$0x00;
            * inst0_arg = arg_u8;

            * inst1_set = true;
            * inst1 = lang$ic$copy$0x00$cell;
            * inst1_arg = eod.data;

            return err$OK;

        }

    }
    
    // actual instruction

    * inst0_arg = arg_u8;

    if(strlen_sameas_cstr(inst, inst_len, "out$arg")){
        * inst0_set = true;
        * inst0 = lang$ic$out$arg;
        return err$OK;
    }else if(strlen_sameas_cstr(inst, inst_len, "out$cell")){
        * inst0_set = true;
        * inst0 = lang$ic$out$cell;
        return err$OK;
    }

    // error

    out$cstr("unknown instruction `");
    out$strlen(inst, inst_len);
    out$cstr("`\n");

    return err$ERR;
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

            UNP(pcd_err, bool, is_compiler_directive, lang$compiler_t$process_directive(&compiler, inst, inst_len, arg, arg_len));

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
                    &compiler, inst, inst_len, arg, arg_len,
                    &inst0_set, &inst0, &inst0_arg,
                    &inst1_set, &inst1, &inst1_arg
                );

                if(err){
                    return err$ERR;
                }

                if(inst0_set){

                    if(ic_code_len >= ic_code_cap){
                        out$cstr("bytecode capacity reached (instruction)\n");
                        return err$ERR;
                    }

                    ic_code[ic_code_len++] = inst0;

                    if(ic_code_len >= ic_code_cap){
                        out$cstr("bytecode capacity reached (argument)\n");
                        return err$ERR;
                    }

                    ic_code[ic_code_len++] = inst0_arg;

                }

                if(inst1_set){

                    if(ic_code_len >= ic_code_cap){
                        out$cstr("bytecode capacity reached (instruction)\n");
                        return err$ERR;
                    }

                    ic_code[ic_code_len++] = inst1;

                    if(ic_code_len >= ic_code_cap){
                        out$cstr("bytecode capacity reached (argument)\n");
                        return err$ERR;
                    }

                    ic_code[ic_code_len++] = inst1_arg;

                }

            }

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
