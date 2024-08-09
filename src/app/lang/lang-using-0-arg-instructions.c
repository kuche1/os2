
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
    return err$ok;
}

err_t lang$inst$add_48_to_0x01(lang$program_data_t * ctx){
    ctx->mem[0x01] += 48;
    return err$ok;
}

err_t lang$inst$sub_48_0x01(lang$program_data_t * ctx){
    ctx->mem[0x01] -= 48;
    return err$ok;
}

err_t lang$inst$putchar_0x01(lang$program_data_t * ctx){
    uint8_t ch = ctx->mem[0x01];
    out$ch((char) ch);
    return err$ok;
}

err_t lang$inst$getchar_0x01(lang$program_data_t * ctx){
    char ch = in$ch();
    ctx->mem[0x01] = (unsigned char) ch;
    return err$ok;
}

err_t lang$inst$clear_0x01(lang$program_data_t * ctx){
    ctx->mem[0x01] = 0;
    return err$ok;
}

err_t lang$inst$add_1_to_0x02(lang$program_data_t * ctx){
    ctx->mem[0x02] += 1;
    return err$ok;
}

err_t lang$inst$copy_0x01_to_addr0x02(lang$program_data_t * ctx){
    size_t addr = ctx->mem[0x02];
    if(addr >= LENOF(ctx->mem)){
        return err$err;
    }
    ctx->mem[addr] = ctx->mem[0x01];
    return err$ok;
}

err_t lang$inst$copy_addr0x02_to_0x01(lang$program_data_t * ctx){
    size_t addr = ctx->mem[0x02];
    if(addr >= LENOF(ctx->mem)){
        return err$err;
    }
    ctx->mem[0x01] = ctx->mem[addr];
    return err$ok;
}

err_t lang$inst$0x01_add_addr0x02_to_0x01(lang$program_data_t * ctx){
    size_t addr = ctx->mem[0x02];
    if(addr >= LENOF(ctx->mem)){
        return err$err;
    }
    ctx->mem[0x01] = ctx->mem[0x01] + ctx->mem[addr];
    return err$ok;
}

err_t lang$inst$put_lca(__attribute__((unused)) lang$program_data_t * ctx){
    out$ch('a');
    return err$ok;
}

err_t lang$inst$put_lcb(__attribute__((unused)) lang$program_data_t * ctx){
    out$ch('b');
    return err$ok;
}

err_t lang$inst$put_colon(__attribute__((unused)) lang$program_data_t * ctx){
    out$ch(':');
    return err$ok;
}

err_t lang$inst$put_nl(__attribute__((unused)) lang$program_data_t * ctx){
    out$ch('\n');
    return err$ok;
}

typedef err_t (* lang$instruction_function_t) (lang$program_data_t *);

lang$instruction_function_t lang$instruction_lookup[] = {
    // print hardcoded
    lang$inst$put_lca,
    lang$inst$put_lcb,
    lang$inst$put_colon,
    lang$inst$put_nl,

    // print variable
    lang$inst$putchar_0x01,

    // input
    lang$inst$getchar_0x01,

    // operations on 0x01
    lang$inst$add_1_to_0x01,
    lang$inst$add_48_to_0x01,
    lang$inst$sub_48_0x01,
    lang$inst$clear_0x01,

    // operations on 0x02
    lang$inst$add_1_to_0x02,

    // operations on both 0x01 and 0x02
    lang$inst$copy_0x01_to_addr0x02,
    lang$inst$copy_addr0x02_to_0x01,
    lang$inst$0x01_add_addr0x02_to_0x01,

    // lang$inst$if_0x01_inc_inst_idx_by_addr0x02, // TODO
};

typedef enum{
    lang$inst$PUT_LCA = 0,
    lang$inst$PUT_LCB,
    lang$inst$PUT_COLON,
    lang$inst$PUT_NL,

    lang$inst$PUTCHAR_0x01,

    lang$inst$GETCHAR_0x01,

    lang$inst$ADD_1_TO_0x01,
    lang$inst$ADD_48_TO_0x01,
    lang$inst$SUB_48_0x01,
    lang$inst$CLEAR_0x01,

    lang$inst$ADD_1_TO_0x02,

    lang$inst$COPY_0x01_TO_ADDR0x02,
    lang$inst$COPY_ADDR0x02_TO_0x01,
    lang$inst$0x01_ADD_ADDR0x02_TO_0x01,
}lang$instruction_t;

// TODO comptile assert that `lang$instruction_t` and `lang$instruction_lookup` are of the same length

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

err_t lang$program_data_t$exec(lang$program_data_t * ctx, size_t number_of_instructions_to_exec, bool * out_execution_finished){

    while(number_of_instructions_to_exec-- > 0){

        if(ctx->instruction_index >= ctx->code_len){
            * out_execution_finished = true;
            return err$ok;
        }

        size_t inst_idx = ctx->instruction_index++;

        lang$instruction_t inst = ctx->code[inst_idx];

        if(inst >= LENOF(lang$instruction_lookup)){
            * out_execution_finished = true;
            return err$err;
        }

        lang$instruction_function_t fnc = lang$instruction_lookup[inst];

        err_t instruction_failure = fnc(ctx);

        if(instruction_failure){
            out$cstr("[instruction failure]\n");
            ctx->instruction_index = ctx->code_len;
            * out_execution_finished = true;
            return err$err;
        }

    }

    * out_execution_finished = false;
    return err$ok;
}

///
////// main
///

err_t lang$main(void){

    // TODO this is insane, we need to make instructions that actually take arguments
    // (we could only do instructions that only take 1 argument, that's fine)
    // // ask user for num0 & operator & num1, then calculates the result
    // lang$instruction_t code[] = {

    //     // "a:"
    //     lang$inst$PUT_LCA,
    //     lang$inst$PUT_COLON,
    //     lang$inst$PUT_NL,

    //     // 0x02 = 4
    //     lang$inst$ADD_1_TO_0x02,
    //     lang$inst$ADD_1_TO_0x02,
    //     lang$inst$ADD_1_TO_0x02,
    //     lang$inst$ADD_1_TO_0x02,
    //     // 0x04 = <a> (the appropriate number value)
    //     lang$inst$GETCHAR_ADDR0x02,
    //     lang$inst$SUB_ORD0_ADDR0x02,

    //     // "op:"
    //     lang$inst$PUT_LCO,
    //     lang$inst$PUT_LCP,
    //     lang$inst$PUT_COLON,
    //     lang$inst$PUT_NL,

    //     // 0x02 = 5
    //     lang$inst$ADD_1_TO_0x02,
    //     // 0x05 = <op> (the ascii value)
    //     lang$inst$GETCHAR_ADDR0x02,
    //     // // 0x03 = 1
    //     // lang$inst$ADD_1_to_0x03,

    //     // "b:"
    //     lang$inst$PUT_LCB,
    //     lang$inst$PUT_COLON,
    //     lang$inst$PUT_NL,

    //     // 0x02 = 6
    //     lang$inst$ADD_1_TO_0x02,
    //     // 0x06 = <b> (the appropriate number value)
    //     lang$inst$GETCHAR_ADDR0x02,
    //     lang$inst$SUB_ORD0_ADDR0x02,

    //     // 0x02 = 5
    //     lang$inst$SUB_1_0x02,
    //     // 0x03 = 1
    //     lang$inst$ADD_1_0x03,
    //     // 0x01 = <op> (ascii)
    //     lang$inst$COPY_ADDR0x02_ADDR0x03,

    //     // 0x01 -= ord('+')
    //     lang$inst$SUB_0x01_ORDPLUS,

    //     // if(0x01 != 0){skip}
    //     lang$inst$if_0x01_inc_inst_idx_by_addr0x02,

    // }

    // ask user for 2 numbers, then add them (only makes sense for up to sum of 9)
    lang$instruction_t code[] = {

        // "a:"
        lang$inst$PUT_LCA,
        lang$inst$PUT_COLON,
        lang$inst$PUT_NL,

        // 0x01 = getchar()
        lang$inst$GETCHAR_0x01,

        // 0x02 = 5
        lang$inst$ADD_1_TO_0x02,
        lang$inst$ADD_1_TO_0x02,
        lang$inst$ADD_1_TO_0x02,
        lang$inst$ADD_1_TO_0x02,
        lang$inst$ADD_1_TO_0x02,

        // * 0x02 = [value of] 0x01
        // 0x05 = <some char>
        lang$inst$COPY_0x01_TO_ADDR0x02,

        // "b:"
        lang$inst$PUT_LCB,
        lang$inst$PUT_COLON,
        lang$inst$PUT_NL,

        // 0x01 = getchar()
        lang$inst$GETCHAR_0x01,

        // 0x01 = [value of] 0x01 - (* 0x02)
        // 0x01 = <some char> - <another char>
        lang$inst$0x01_ADD_ADDR0x02_TO_0x01,

        // 0x01 -= 48
        lang$inst$SUB_48_0x01,

        // print([value of] 0x01)
        lang$inst$PUTCHAR_0x01,
        lang$inst$PUT_NL,

    };

    // print 0102
    // lang$instruction_t code[] = {
    //     lang$INST_ADD_48_TO_0x01,
    //     lang$INST_PUTCHAR_0x01,

    //     lang$INST_ADD_1_TO_0x01,
    //     lang$INST_PUTCHAR_0x01,

    //     lang$INST_ADD_1_TO_0x02,
    //     lang$INST_ADD_1_TO_0x02,
    //     lang$INST_ADD_1_TO_0x02,
    //     lang$INST_ADD_1_TO_0x02,
    //     lang$INST_ADD_1_TO_0x02,

    //     lang$INST_ADD_1_TO_0x01,
    //     // lang$INST_PUTCHAR_0x01,
    //     lang$INST_COPY_0x01_TO_ADDR0x02,
    //     lang$INST_CLEAR_0x01,
    //     lang$INST_ADD_48_TO_0x01,
    //     lang$INST_PUTCHAR_0x01,
    //     lang$INST_COPY_ADDR0x02_TO_0x01,
    //     lang$INST_PUTCHAR_0x01,
    // };

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

    return err$ok;

}
