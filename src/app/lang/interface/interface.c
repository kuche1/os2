
err_t lang$program_data_t$init_from_instruction_code(lang$program_data_t * ctx, uint8_t * code, size_t code_len){
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

#include "init_from_cstr.c"

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
