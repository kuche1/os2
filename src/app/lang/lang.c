
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

#include "instructions.c"

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
