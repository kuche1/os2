
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

    uint8_t * code; // TODO use `uint16_t` instead, so that we can access 2**16 addresses instead of 2**8; then bring back the addr check ifs in the instructions
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

#include "interface/interface.c"

///
////// main
///

err_t lang$main(void){

    COMPTIME_ASSERT(lang$ic$len == LENOF(lang$instruction_lookup));
    // this only reason this is here is because it doesn't work in global scope

    lang$program_data_t context;


    char cstrcode[] =
        // #include "example-program/cstr/hello-world.c"
        // #include "example-program/cstr/variable.c"
        // #include "example-program/cstr/sum.c"
        // #include "example-program/cstr/calc.c"
        #include "example-program/cstr/calc-better.c"
    ;

    size_t bytecode_maxlen = 320;

    uint8_t bytecode[bytecode_maxlen];

    {
        err_t err = lang$program_data_t$init_from_cstr(&context, cstrcode, bytecode, bytecode_maxlen);
        if(err){
            return err;
        }
    }

    // uint8_t bytecode[] =
    //     #include "example-program/bytecode/calc.c"
    //     // #include "example-program/bytecode/hello-world.c"
    // ;

    // {
    //     err_t err = lang$program_data_t$init_from_instruction_code(&context, bytecode, LENOF(bytecode));
    //     if(err){
    //         return err;
    //     }
    // }

    while(true){
    
        bool exec_finished;
        err_t err = lang$program_data_t$exec(&context, 1, &exec_finished);

        if(err){
            return err;
        }

        if(exec_finished){
            break;
        }

    }

    return err$ok;

}
