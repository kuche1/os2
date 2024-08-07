
typedef enum{
    lang$INST_SET_PTR,
    lang$INST_SET_U32,
    lang$INST_PUT_CHAR,
    lang$INST_ADD_U32,
}lang$instruction_t;

typedef struct{
    uint8_t mem[1024 * 10];
    size_t mem_ptr;
}lang$program_data_t;

inline err_t lang$set_mem_ptr(lang$program_data_t * ctx, size_t addr){
    // if(addr < 0){ // no need since addr is unsigned
    //     return err$ERR;
    // }
    if(addr >= LENOF(ctx->mem)){
        return err$ERR;
    }
    ctx->mem_ptr = addr;
    return err$OK;
}

inline uint32_t lang$get_u32(lang$program_data_t * ctx){
    return * (uint32_t *) & ctx->mem[ctx->mem_ptr];
}

inline void lang$set_u32(lang$program_data_t * ctx, uint32_t value){
    * (uint32_t *) & ctx->mem[ctx->mem_ptr] = value;
}

inline void lang$put_char(lang$program_data_t * ctx){
    unsigned char ch = ctx->mem[ctx->mem_ptr];
    out$ch((char) ch);
}

inline void lang$add_u32(lang$program_data_t * ctx, uint32_t num){
    lang$set_u32(ctx, lang$get_u32(ctx) + num);
}

err_t lang$main(void){

    lang$program_data_t context;
    lang$program_data_t * ctx = & context;

    for(size_t i=0; i<LENOF(ctx->mem); ++i){
        ctx->mem[i] = 0;
    }

    ctx->mem_ptr = 0;

    uint32_t code[] = {
        lang$INST_SET_PTR,
        0x40u,

        lang$INST_SET_U32,
        69,

        lang$INST_PUT_CHAR,

        lang$INST_ADD_U32,
        1u,

        lang$INST_PUT_CHAR,
    };

    for(size_t idx=0; idx<LENOF(code); ++idx){
        lang$instruction_t instruction = code[idx];

        switch(instruction){

            case lang$INST_SET_PTR:{
                uint32_t addr = code[++idx]; // possible out of bound
                err_t err = lang$set_mem_ptr(ctx, addr);
                if(err){
                    out$cstr("attempt to set the pointer to invalid location\n");
                    return err;
                }
            }break;

            case lang$INST_SET_U32:{
                uint32_t value = code[++idx]; // possible out of bound
                lang$set_u32(ctx, value);
            }break;

            case lang$INST_PUT_CHAR:{
                lang$put_char(ctx);
            }break;

            case lang$INST_ADD_U32:{
                uint32_t num = code[++idx]; // possible out of bound
                lang$add_u32(ctx, num);
            }break;

        }
    }

    return err$OK;

}
