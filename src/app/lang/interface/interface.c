
err_t lang$program_data_t$init_from_instruction_code(lang$program_data_t * ctx, uint8_t * code, size_t code_len){
    for(size_t i=0; i<LENOF(ctx->mem); ++i){
        ctx->mem[i] = 0;
    }

    ctx->code = code;
    ctx->code_len = code_len;
    if(code_len % lang$INSTRUCTION_SIZE != 0){
        out$cstr("bad code length\n");
        return err$err;
    }

    ctx->instruction_index = 0;

    return err$ok;
}

#include "init_from_cstr.c"

err_t lang$program_data_t$exec(lang$program_data_t * ctx, size_t number_of_instructions_to_exec, bool * out_execution_finished){

    while(number_of_instructions_to_exec-- > 0){

        if(ctx->instruction_index >= ctx->code_len){
            * out_execution_finished = true;
            return err$ok;
        }

        lang$instruction_code_t inst = ctx->code[ctx->instruction_index++];
        uint8_t arg = ctx->code[ctx->instruction_index++];

        if(inst >= LENOF(lang$instruction_lookup)){
            * out_execution_finished = true;
            return err$err;
        }

        lang$instruction_function_t fnc = lang$instruction_lookup[inst];

        err_t instruction_failure = fnc(ctx, arg);

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
