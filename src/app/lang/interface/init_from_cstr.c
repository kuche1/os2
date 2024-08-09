
#define lang$init_from_cstr$WORD_MAXLEN 10

#define lang$init_from_cstr$INST_MAX_ARGS 10

#include "compiler.c"

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

            bool inst0_set = false;
            uint8_t inst0;
            uint8_t inst0_arg;

            bool inst1_set = false;
            uint8_t inst1;
            uint8_t inst1_arg;

            bool inst2_set = false;
            uint8_t inst2;
            uint8_t inst2_arg;

            err_t err = lang$compiler_t$compile_instruction(
                &compiler,
                inst, inst_len,
                arguments, argument_lens, arguments_len,
                &inst0_set, &inst0, &inst0_arg,
                &inst1_set, &inst1, &inst1_arg,
                &inst2_set, &inst2, &inst2_arg
            );

            if(err){
                return err;
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

            if(inst2_set){

                if(ic_code_len >= ic_code_cap){
                    out$cstr("bytecode capacity reached (instruction)\n");
                    return err$err;
                }

                ic_code[ic_code_len++] = inst2;

                if(ic_code_len >= ic_code_cap){
                    out$cstr("bytecode capacity reached (argument)\n");
                    return err$err;
                }

                ic_code[ic_code_len++] = inst2_arg;

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

    if((word_len != 0) || (inst_len != 0) || (arguments_len != 0)){
        out$cstr("something went wrong\n");
        return err$err;
    }

    return lang$program_data_t$init_from_instruction_code(ctx, ic_code, ic_code_len);

}
