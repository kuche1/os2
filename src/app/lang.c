
// would be super cool if we could make it so that each instruction is an
// "offset" in an array of function pointers, so that we can execute the
// function right away without any ifs or buts

// as for the "instructions", we could instead of having an instruction
// take a look into the next instruction and take it as an argument,
// we could use some addresses the way CPUs use rax, rbx, ...
// say, 0x00, 0x01, 0x02, ...

// typedef enum{
//     lang$INST_SET_PTR,
//     lang$INST_SET_U32,
//     lang$INST_PUT_CHAR,
//     lang$INST_ADD_U32,
//     // lang$INST_GET_CHAR,
// }lang$instruction_t;

typedef struct{
    uint8_t mem[1024 * 10]; // it is assumed that there is enough space for the first couple of necessary "registers"

    uint32_t * code;
    size_t code_len;

    size_t instruction_index;
}lang$program_data_t;

///
////// instructions
///

void lang$inst$add_1_to_0x01(lang$program_data_t * ctx){
    ctx->mem[0x01] += 1;
}

void lang$inst$add_48_to_0x01(lang$program_data_t * ctx){
    ctx->mem[0x01] += 48;
}

void lang$inst$putchar_0x01(lang$program_data_t * ctx){
    uint8_t ch = ctx->mem[0x01];
    out$ch((char) ch);
}

// inline uint32_t lang$get_u32(lang$program_data_t * ctx){
//     return * (uint32_t *) & ctx->mem[ctx->mem_ptr];
// }

// inline void lang$set_u32(lang$program_data_t * ctx, uint32_t value){
//     * (uint32_t *) & ctx->mem[ctx->mem_ptr] = value;
// }

// inline void lang$put_char(lang$program_data_t * ctx){
//     unsigned char ch = ctx->mem[ctx->mem_ptr];
//     out$ch((char) ch);
// }

// inline void lang$add_u32(lang$program_data_t * ctx, uint32_t num){
//     lang$set_u32(ctx, lang$get_u32(ctx) + num);
// }

typedef void (* lang$instruction_function_t) (lang$program_data_t *);

lang$instruction_function_t lang$instruction_lookup[] = {
    lang$inst$add_1_to_0x01,
    lang$inst$add_48_to_0x01,
    lang$inst$putchar_0x01,
};

typedef enum{
    lang$INST_ADD_1_TO_0x01 = 0,
    lang$INST_ADD_48_TO_0x01,
    lang$INST_PUTCHAR_0x01,
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
bool lang$program_data_t$exec(lang$program_data_t * ctx, size_t number_of_instructions_to_exec){

    while(number_of_instructions_to_exec-- > 0){

        if(ctx->instruction_index >= ctx->code_len){
            return true;
        }

        size_t inst_idx = ctx->instruction_index++;

        lang$instruction_t inst = ctx->code[inst_idx];

        lang$instruction_function_t fnc = lang$instruction_lookup[inst]; // TODO check if out of bound

        fnc(ctx);
    }

    return false;
}

///
////// main
///

err_t lang$main(void){

    // uint32_t code[] = {
    //     lang$INST_SET_PTR,
    //     0x40u,

    //     lang$INST_SET_U32,
    //     69,

    //     lang$INST_PUT_CHAR,

    //     lang$INST_ADD_U32,
    //     1u,

    //     lang$INST_PUT_CHAR,
    // };

    lang$instruction_t code[] = {
        lang$INST_ADD_48_TO_0x01,
        lang$INST_PUTCHAR_0x01,

        lang$INST_ADD_1_TO_0x01,
        lang$INST_PUTCHAR_0x01,
    };

    lang$program_data_t context;
    lang$program_data_t * ctx = & context;
    lang$program_data_t$init(ctx, code, LENOF(code));

    while(!lang$program_data_t$exec(ctx, 1)){}

    // for(size_t idx=0; idx<LENOF(code); ++idx){
    //     lang$instruction_t instruction = code[idx];

    //     switch(instruction){

    //         case lang$INST_SET_PTR:{
    //             uint32_t addr = code[++idx]; // possible out of bound
    //             err_t err = lang$set_mem_ptr(ctx, addr);
    //             if(err){
    //                 out$cstr("attempt to set the pointer to invalid location\n");
    //                 return err;
    //             }
    //         }break;

    //         case lang$INST_SET_U32:{
    //             uint32_t value = code[++idx]; // possible out of bound
    //             lang$set_u32(ctx, value);
    //         }break;

    //         case lang$INST_PUT_CHAR:{
    //             lang$put_char(ctx);
    //         }break;

    //         case lang$INST_ADD_U32:{
    //             uint32_t num = code[++idx]; // possible out of bound
    //             lang$add_u32(ctx, num);
    //         }break;

    //     }
    // }

    return err$OK;

}
