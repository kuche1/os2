
typedef struct{
    uint8_t mem[1024 * 10]; // it is assumed that there is enough space for the first couple of necessary "registers"

    uint32_t * code;
    size_t code_len;

    size_t instruction_index;
}lang$program_data_t;

///
////// instructions
///

err_t lang$inst$add_1_to_0x01(lang$program_data_t * ctx){
    ctx->mem[0x01] += 1;
    return err$OK;
}

err_t lang$inst$add_48_to_0x01(lang$program_data_t * ctx){
    ctx->mem[0x01] += 48;
    return err$OK;
}

err_t lang$inst$putchar_0x01(lang$program_data_t * ctx){
    uint8_t ch = ctx->mem[0x01];
    out$ch((char) ch);
    return err$OK;
}

err_t lang$inst$clear_0x01(lang$program_data_t * ctx){
    ctx->mem[0x01] = 0;
    return err$OK;
}

err_t lang$inst$add_1_to_0x02(lang$program_data_t * ctx){
    ctx->mem[0x02] += 1;
    return err$OK;
}

err_t lang$inst$copy_0x01_to_addr0x02(lang$program_data_t * ctx){
    size_t addr = ctx->mem[0x02];
    if(addr >= LENOF(ctx->mem)){
        return err$ERR;
    }
    ctx->mem[addr] = ctx->mem[0x01];
    return err$OK;
}

err_t lang$inst$copy_addr0x02_to_0x01(lang$program_data_t * ctx){
    size_t addr = ctx->mem[0x02];
    if(addr >= LENOF(ctx->mem)){
        return err$ERR;
    }
    ctx->mem[0x01] = ctx->mem[addr];
    return err$OK;
}

typedef err_t (* lang$instruction_function_t) (lang$program_data_t *);

lang$instruction_function_t lang$instruction_lookup[] = {
    // only 0x01
    lang$inst$add_1_to_0x01,
    lang$inst$add_48_to_0x01,
    lang$inst$putchar_0x01,
    lang$inst$clear_0x01,

    // only 0x02
    lang$inst$add_1_to_0x02,

    // 0x01 and 0x02
    lang$inst$copy_0x01_to_addr0x02,
    lang$inst$copy_addr0x02_to_0x01,
};

typedef enum{
    lang$INST_ADD_1_TO_0x01 = 0,
    lang$INST_ADD_48_TO_0x01,
    lang$INST_PUTCHAR_0x01,
    lang$INST_CLEAR_0x01,

    lang$INST_ADD_1_TO_0x02,

    lang$INST_COPY_0x01_TO_ADDR0x02,
    lang$INST_COPY_ADDR0x02_TO_0x01,
}lang$instruction_t;

///
////// interface
///

void lang$program_data_t$init(lang$program_data_t * ctx, lang$instruction_t * code, size_t code_len){
    for(size_t i=0; i<LENOF(ctx->mem); ++i){
        ctx->mem[i] = 0;
    }

    ctx->code = (uint32_t *) code;
    ctx->code_len = code_len;

    ctx->instruction_index = 0;
}

// true - execution finished
err_or_bool_t lang$program_data_t$exec(lang$program_data_t * ctx, size_t number_of_instructions_to_exec){

    while(number_of_instructions_to_exec-- > 0){

        if(ctx->instruction_index >= ctx->code_len){
            return (err_or_bool_t) {.err = err$OK, .data = true};
        }

        size_t inst_idx = ctx->instruction_index++;

        lang$instruction_t inst = ctx->code[inst_idx];

        if(inst >= LENOF(lang$instruction_lookup)){
            return (err_or_bool_t) {.err = err$ERR, .data = true};
        }

        lang$instruction_function_t fnc = lang$instruction_lookup[inst];

        err_t instruction_failure = fnc(ctx);

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

    lang$instruction_t code[] = {
        lang$INST_ADD_48_TO_0x01,
        lang$INST_PUTCHAR_0x01,

        lang$INST_ADD_1_TO_0x01,
        lang$INST_PUTCHAR_0x01,

        lang$INST_ADD_1_TO_0x02,
        lang$INST_ADD_1_TO_0x02,
        lang$INST_ADD_1_TO_0x02,
        lang$INST_ADD_1_TO_0x02,
        lang$INST_ADD_1_TO_0x02,

        lang$INST_ADD_1_TO_0x01,
        // lang$INST_PUTCHAR_0x01,
        lang$INST_COPY_0x01_TO_ADDR0x02,
        lang$INST_CLEAR_0x01,
        lang$INST_ADD_48_TO_0x01,
        lang$INST_PUTCHAR_0x01,
        lang$INST_COPY_ADDR0x02_TO_0x01,
        lang$INST_PUTCHAR_0x01,
    };

    lang$program_data_t context;
    lang$program_data_t * ctx = & context;
    lang$program_data_t$init(ctx, code, LENOF(code));

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
