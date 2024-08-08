
// TODO
//
// would be super cool if we could make it so that the interperter
// goes over the source code and finds any "repetitions" of instructions
// and creates a new instruction that is basically both of these but into
// one
//
// the parser could simply look for names that exactly match the instruction codes
// but without the `lang$ic$` prefix
//
// if we are to "compile" these programs, instead of writing each of the codes
// to a file, we could write the length, and the "index" of the program, given that
// we know all possible programs given the length
// given that we know the number of instructions, we can calculate how much instructions
// we can cram in, say 64 bits, then chop the program into multiple 64bit pieces (this could
// also remove the need to write the program length, if we insteam used a worthless instruction
// that fills the last "block" (if needed))

#define lang$INSTRUCTION_SIZE 2
// 1 - the instruction
// 2 - the argument

typedef struct{
    uint8_t mem[1024 * 10]; // it is assumed that there is enough space for the first couple of necessary "registers"

    uint8_t * code; // TODO use `uint16_t` instead, so that we can access 2**16 addresses instead of 2**8; then bring back the ifs in the instructions
    size_t code_len;

    size_t instruction_index;
}lang$program_data_t;

///
////// instructions
///

err_t lang$if$out$arg(__attribute__((unused)) lang$program_data_t * ctx, uint8_t arg){
    out$ch((char) arg);
    return err$OK;
}

err_t lang$if$out$cell(lang$program_data_t * ctx, uint8_t arg){
    // if(arg >= LENOF(ctx->mem)){
    //     return err$ERR;
    // }
    uint8_t value = ctx->mem[arg];
    out$ch((char) value);
    return err$OK;
}

err_t lang$if$in$cell(lang$program_data_t * ctx, uint8_t arg){
    // if(arg >= LENOF(ctx->mem)){
    //     return err$ERR;
    // }
    char ch = in$ch();
    ctx->mem[arg] = (uint8_t) ch;
    return err$OK;
}

err_t lang$if$copy$cell$0x00(lang$program_data_t * ctx, uint8_t arg){
    // if(arg >= LENOF(ctx->mem)){
    //     return err$ERR;
    // }
    uint8_t val = ctx->mem[arg];
    ctx->mem[0x00] = val;
    return err$OK;
}

err_t lang$if$copy$0x00$cell(lang$program_data_t * ctx, uint8_t arg){
    // if(arg >= LENOF(ctx->mem)){
    //     return err$ERR;
    // }
    uint8_t val = ctx->mem[0x00];
    ctx->mem[arg] = val;
    return err$OK;
}

err_t lang$if$add$0x00$cell(lang$program_data_t * ctx, uint8_t arg){
    // if(arg >= LENOF(ctx->mem)){
    //     return err$ERR;
    // }
    uint8_t val = ctx->mem[arg];
    ctx->mem[0x00] += val;
    return err$OK;
}

err_t lang$if$sub$0x00$arg(lang$program_data_t * ctx, uint8_t arg){
    ctx->mem[0x00] -= arg;
    return err$OK;
}

err_t lang$if$add$0x00$arg(lang$program_data_t * ctx, uint8_t arg){
    ctx->mem[0x00] += arg;
    return err$OK;
}

err_t lang$if$if$0x00$skipinst$arg(lang$program_data_t * ctx, uint8_t arg){
    if(ctx->mem[0x00]){
        ctx->instruction_index += arg * lang$INSTRUCTION_SIZE;
    }
    return err$OK;
}

err_t lang$if$sub$0x00$cell(lang$program_data_t * ctx, uint8_t arg){
    // if(arg >= LENOF(ctx->mem)){
    //     return err$ERR;
    // }
    ctx->mem[0x00] -= ctx->mem[arg];
    return err$OK;
}

err_t lang$if$mul$0x00$cell(lang$program_data_t * ctx, uint8_t arg){
    // if(arg >= LENOF(ctx->mem)){
    //     return err$ERR;
    // }
    ctx->mem[0x00] *= ctx->mem[arg];
    return err$OK;
}

err_t lang$if$div$0x00$cell(lang$program_data_t * ctx, uint8_t arg){
    // if(arg >= LENOF(ctx->mem)){
    //     return err$ERR;
    // }
    uint8_t val = ctx->mem[arg];
    if(val == 0){ // not sure if this is the correct cource of action
        return err$ERR;
    }
    ctx->mem[0x00] /= val;
    return err$OK;
}

typedef err_t (* lang$instruction_function_t) (lang$program_data_t *, uint8_t);

lang$instruction_function_t lang$instruction_lookup[] = {

    // output
    lang$if$out$arg,
    lang$if$out$cell,
    // lang$if$out$ptrcell,

    // // input
    lang$if$in$cell,
    // lang$if$in$ptrcell,

    // // copy from
    lang$if$copy$cell$0x00,
    // lang$if$copy$ptrcell$0x00,
    // lang$if$copy$cell$0x01,
    // lang$if$copy$ptrcell$0x01,
    // lang$if$copy$cell$0x02,
    // lang$if$copy$ptrcell$0x02,
    // lang$if$copy$cell$0x03,
    // lang$if$copy$ptrcell$0x03,

    // // copy to
    lang$if$copy$0x00$cell,
    // lang$if$copy$0x00$ptrcell,
    // lang$if$copy$0x01$cell,
    // lang$if$copy$0x01$ptrcell,
    // lang$if$copy$0x02$cell,
    // lang$if$copy$0x02$ptrcell,
    // lang$if$copy$0x03$cell,
    // lang$if$copy$0x03$ptrcell,

    // //// arithmetic
    // // 0x00
    lang$if$add$0x00$arg,
    lang$if$add$0x00$cell,
    // lang$if$add$0x00$ptrcell,
    lang$if$sub$0x00$arg,
    lang$if$sub$0x00$cell,
    // lang$if$sub$0x00$ptrcell,
    // lang$if$mul$0x00$arg,
    lang$if$mul$0x00$cell,
    // lang$if$mul$0x00$ptrcell,
    // lang$if$div$0x00$arg,
    lang$if$div$0x00$cell,
    // lang$if$div$0x00$ptrcell,
    // // 0x01
    // lang$if$add$0x01$arg,
    // lang$if$add$0x01$cell,
    // lang$if$add$0x01$ptrcell,
    // lang$if$sub$0x01$arg,
    // lang$if$sub$0x01$cell,
    // lang$if$sub$0x01$ptrcell,
    // lang$if$mul$0x01$arg,
    // lang$if$mul$0x01$cell,
    // lang$if$mul$0x01$ptrcell,
    // lang$if$div$0x01$arg,
    // lang$if$div$0x01$cell,
    // lang$if$div$0x01$ptrcell,
    // // 0x02
    // lang$if$add$0x02$arg,
    // lang$if$add$0x02$cell,
    // lang$if$add$0x02$ptrcell,
    // lang$if$sub$0x02$arg,
    // lang$if$sub$0x02$cell,
    // lang$if$sub$0x02$ptrcell,
    // lang$if$mul$0x02$arg,
    // lang$if$mul$0x02$cell,
    // lang$if$mul$0x02$ptrcell,
    // lang$if$div$0x02$arg,
    // lang$if$div$0x02$cell,
    // lang$if$div$0x02$ptrcell,
    // // 0x03
    // lang$if$add$0x03$arg,
    // lang$if$add$0x03$cell,
    // lang$if$add$0x03$ptrcell,
    // lang$if$sub$0x03$arg,
    // lang$if$sub$0x03$cell,
    // lang$if$sub$0x03$ptrcell,
    // lang$if$mul$0x03$arg,
    // lang$if$mul$0x03$cell,
    // lang$if$mul$0x03$ptrcell,
    // lang$if$div$0x03$arg,
    // lang$if$div$0x03$cell,
    // lang$if$div$0x03$ptrcell,

    // // if
    lang$if$if$0x00$skipinst$arg,
    // lang$if$if$0x00$skipinst$cell,
    // lang$if$if$0x00$skipinst$ptrcell,
    // lang$if$if$0x01$skipinst$arg,
    // lang$if$if$0x01$skipinst$cell,
    // lang$if$if$0x01$skipinst$ptrcell,
    // lang$if$if$0x02$skipinst$arg,
    // lang$if$if$0x02$skipinst$cell,
    // lang$if$if$0x02$skipinst$ptrcell,
    // lang$if$if$0x03$skipinst$arg,
    // lang$if$if$0x03$skipinst$cell,
    // lang$if$if$0x03$skipinst$ptrcell,

};

typedef enum{

    // output
    lang$ic$out$arg,
    lang$ic$out$cell,
    // lang$ic$out$ptrcell,

    // // input
    lang$ic$in$cell,
    // lang$ic$in$ptrcell,

    // // copy from
    lang$ic$copy$cell$0x00,
    // lang$ic$copy$ptrcell$0x00,
    // lang$ic$copy$cell$0x01,
    // lang$ic$copy$ptrcell$0x01,
    // lang$ic$copy$cell$0x02,
    // lang$ic$copy$ptrcell$0x02,
    // lang$ic$copy$cell$0x03,
    // lang$ic$copy$ptrcell$0x03,

    // // copy to
    lang$ic$copy$0x00$cell,
    // lang$ic$copy$0x00$ptrcell,
    // lang$ic$copy$0x01$cell,
    // lang$ic$copy$0x01$ptrcell,
    // lang$ic$copy$0x02$cell,
    // lang$ic$copy$0x02$ptrcell,
    // lang$ic$copy$0x03$cell,
    // lang$ic$copy$0x03$ptrcell,

    // //// arithmetic
    // // 0x00
    lang$ic$add$0x00$arg,
    lang$ic$add$0x00$cell,
    // lang$ic$add$0x00$ptrcell,
    lang$ic$sub$0x00$arg,
    lang$ic$sub$0x00$cell,
    // lang$ic$sub$0x00$ptrcell,
    // lang$ic$mul$0x00$arg,
    lang$ic$mul$0x00$cell,
    // lang$ic$mul$0x00$ptrcell,
    // lang$ic$div$0x00$arg,
    lang$ic$div$0x00$cell,
    // lang$ic$div$0x00$ptrcell,
    // // 0x01
    // lang$ic$add$0x01$arg,
    // lang$ic$add$0x01$cell,
    // lang$ic$add$0x01$ptrcell,
    // lang$ic$sub$0x01$arg,
    // lang$ic$sub$0x01$cell,
    // lang$ic$sub$0x01$ptrcell,
    // lang$ic$mul$0x01$arg,
    // lang$ic$mul$0x01$cell,
    // lang$ic$mul$0x01$ptrcell,
    // lang$ic$div$0x01$arg,
    // lang$ic$div$0x01$cell,
    // lang$ic$div$0x01$ptrcell,
    // // 0x02
    // lang$ic$add$0x02$arg,
    // lang$ic$add$0x02$cell,
    // lang$ic$add$0x02$ptrcell,
    // lang$ic$sub$0x02$arg,
    // lang$ic$sub$0x02$cell,
    // lang$ic$sub$0x02$ptrcell,
    // lang$ic$mul$0x02$arg,
    // lang$ic$mul$0x02$cell,
    // lang$ic$mul$0x02$ptrcell,
    // lang$ic$div$0x02$arg,
    // lang$ic$div$0x02$cell,
    // lang$ic$div$0x02$ptrcell,
    // // 0x03
    // lang$ic$add$0x03$arg,
    // lang$ic$add$0x03$cell,
    // lang$ic$add$0x03$ptrcell,
    // lang$ic$sub$0x03$arg,
    // lang$ic$sub$0x03$cell,
    // lang$ic$sub$0x03$ptrcell,
    // lang$ic$mul$0x03$arg,
    // lang$ic$mul$0x03$cell,
    // lang$ic$mul$0x03$ptrcell,
    // lang$ic$div$0x03$arg,
    // lang$ic$div$0x03$cell,
    // lang$ic$div$0x03$ptrcell,

    // // if
    lang$ic$if$0x00$skipinst$arg,
    // lang$ic$if$0x00$skipinst$cell,
    // lang$ic$if$0x00$skipinst$ptrcell,
    // lang$ic$if$0x01$skipinst$arg,
    // lang$ic$if$0x01$skipinst$cell,
    // lang$ic$if$0x01$skipinst$ptrcell,
    // lang$ic$if$0x02$skipinst$arg,
    // lang$ic$if$0x02$skipinst$cell,
    // lang$ic$if$0x02$skipinst$ptrcell,
    // lang$ic$if$0x03$skipinst$arg,
    // lang$ic$if$0x03$skipinst$cell,
    // lang$ic$if$0x03$skipinst$ptrcell,

    lang$ic$len,

}lang$instruction_code_t;

///
////// interface
///

err_t lang$program_data_t$init(lang$program_data_t * ctx, uint8_t * code, size_t code_len){
    for(size_t i=0; i<LENOF(ctx->mem); ++i){
        ctx->mem[i] = 0;
    }

    ctx->code = code;
    ctx->code_len = code_len;
    if(code_len % lang$INSTRUCTION_SIZE != 0){
        out$cstr("bad code length\n");
        return err$ERR;
    }

    ctx->instruction_index = 0;

    return err$OK;
}

// true - execution finished
// false - there are more instructions to be executed
err_or_bool_t lang$program_data_t$exec(lang$program_data_t * ctx, size_t number_of_instructions_to_exec){

    while(number_of_instructions_to_exec-- > 0){

        if(ctx->instruction_index >= ctx->code_len){
            return (err_or_bool_t) {.err = err$OK, .data = true};
        }

        lang$instruction_code_t inst = ctx->code[ctx->instruction_index++];
        uint8_t arg = ctx->code[ctx->instruction_index++];

        if(inst >= LENOF(lang$instruction_lookup)){
            return (err_or_bool_t) {.err = err$ERR, .data = true};
        }

        lang$instruction_function_t fnc = lang$instruction_lookup[inst];

        err_t instruction_failure = fnc(ctx, arg);

        if(instruction_failure){
            out$cstr("[instruction failure]\n");
            ctx->instruction_index = ctx->code_len;
            return (err_or_bool_t) {.err = err$ERR, .data = true};
        }

    }

    return (err_or_bool_t) {.err = err$OK, .data = false};
}

///
////// main
///

err_t lang$main(void){

    COMPTIME_ASSERT(lang$ic$len == LENOF(lang$instruction_lookup));
    // this only reason this is here is because it doesn't work in global scope

    uint8_t code[] =
        #include "example-program/calc.c"
    ;

    lang$program_data_t context;
    lang$program_data_t * ctx = & context;
    {
        err_t err = lang$program_data_t$init(ctx, code, LENOF(code));
        if(err){
            return err;
        }
    }

    while(true){

        UNP(err, bool, exec_finished, lang$program_data_t$exec(ctx, 1));

        if(err){
            return err;
        }

        if(exec_finished){
            break;
        }

    }

    return err$OK;

}
