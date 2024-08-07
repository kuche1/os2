
typedef enum{
    lang$INST_SET_PTR,
    lang$INST_SET_U32,
    lang$INST_PUT_CHAR,
    lang$INST_ADD_U32,
}lang$instruction_t;

uint8_t lang$mem[1024 * 20];

size_t lang$mem_ptr;

err_t lang$set_mem_ptr(size_t addr){
    // if(addr < 0){ // no need since addr is unsigned
    //     return err$ERR;
    // }
    if(addr >= LENOF(lang$mem)){
        return err$ERR;
    }
    lang$mem_ptr = addr;
    return err$OK;
}

uint32_t lang$get_u32(void){
    return * (uint32_t *) & lang$mem[lang$mem_ptr];
}

void lang$set_u32(uint32_t value){
    * (uint32_t *) & lang$mem[lang$mem_ptr] = value;
}

void lang$put_char(void){
    unsigned char ch = lang$mem[lang$mem_ptr];
    out$ch((char) ch);
}

void lang$add_u32(uint32_t num){
    lang$set_u32(lang$get_u32() + num);
}

void lang$main(void){

    for(size_t i=0; i<LENOF(lang$mem); ++i){
        lang$mem[i] = 0;
    }

    lang$mem_ptr = 0;

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
                lang$set_mem_ptr(addr); // TODO this could return an error
            }break;

            case lang$INST_SET_U32:{
                uint32_t value = code[++idx]; // possible out of bound
                lang$set_u32(value);
            }break;

            case lang$INST_PUT_CHAR:{
                lang$put_char();
            }break;

            case lang$INST_ADD_U32:{
                uint32_t num = code[++idx]; // possible out of bound
                lang$add_u32(num);
            }break;

        }
    }

}
