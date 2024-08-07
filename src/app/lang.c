
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

    return err$OK;

}
