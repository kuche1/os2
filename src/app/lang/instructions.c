
err_t lang$if$out$arg(__attribute__((unused)) lang$program_data_t * ctx, uint8_t arg){
    out$ch((char) arg);
    return err$OK;
}

err_t lang$if$out$cell(lang$program_data_t * ctx, uint8_t arg){
    // if(arg >= LENOF(ctx->mem)){
    //     return err$ERR;
    // }
    uint8_t value = ctx->mem[arg];
    out$ch((char) value);
    return err$OK;
}

err_t lang$if$in$cell(lang$program_data_t * ctx, uint8_t arg){
    // if(arg >= LENOF(ctx->mem)){
    //     return err$ERR;
    // }
    char ch = in$ch();
    ctx->mem[arg] = (uint8_t) ch;
    return err$OK;
}

err_t lang$if$copy$cell$0x00(lang$program_data_t * ctx, uint8_t arg){
    // if(arg >= LENOF(ctx->mem)){
    //     return err$ERR;
    // }
    uint8_t val = ctx->mem[arg];
    ctx->mem[0x00] = val;
    return err$OK;
}

err_t lang$if$copy$0x00$cell(lang$program_data_t * ctx, uint8_t arg){
    // if(arg >= LENOF(ctx->mem)){
    //     return err$ERR;
    // }
    uint8_t val = ctx->mem[0x00];
    ctx->mem[arg] = val;
    return err$OK;
}

err_t lang$if$add$0x00$cell(lang$program_data_t * ctx, uint8_t arg){
    // if(arg >= LENOF(ctx->mem)){
    //     return err$ERR;
    // }
    uint8_t val = ctx->mem[arg];
    ctx->mem[0x00] += val;
    return err$OK;
}

err_t lang$if$sub$0x00$arg(lang$program_data_t * ctx, uint8_t arg){
    ctx->mem[0x00] -= arg;
    return err$OK;
}

err_t lang$if$add$0x00$arg(lang$program_data_t * ctx, uint8_t arg){
    ctx->mem[0x00] += arg;
    return err$OK;
}

err_t lang$if$if$0x00$skipinst$arg(lang$program_data_t * ctx, uint8_t arg){
    if(ctx->mem[0x00]){
        ctx->instruction_index += arg * lang$INSTRUCTION_SIZE;
    }
    return err$OK;
}

err_t lang$if$sub$0x00$cell(lang$program_data_t * ctx, uint8_t arg){
    // if(arg >= LENOF(ctx->mem)){
    //     return err$ERR;
    // }
    ctx->mem[0x00] -= ctx->mem[arg];
    return err$OK;
}

err_t lang$if$mul$0x00$cell(lang$program_data_t * ctx, uint8_t arg){
    // if(arg >= LENOF(ctx->mem)){
    //     return err$ERR;
    // }
    ctx->mem[0x00] *= ctx->mem[arg];
    return err$OK;
}

err_t lang$if$div$0x00$cell(lang$program_data_t * ctx, uint8_t arg){
    // if(arg >= LENOF(ctx->mem)){
    //     return err$ERR;
    // }
    uint8_t val = ctx->mem[arg];
    if(val == 0){ // not sure if this is the correct cource of action
        return err$ERR;
    }
    ctx->mem[0x00] /= val;
    return err$OK;
}

typedef err_t (* lang$instruction_function_t) (lang$program_data_t *, uint8_t);

lang$instruction_function_t lang$instruction_lookup[] = {

    // output
    lang$if$out$arg,
    lang$if$out$cell,
    // lang$if$out$ptrcell,

    // // input
    lang$if$in$cell,
    // lang$if$in$ptrcell,

    // // copy from
    lang$if$copy$cell$0x00,
    // lang$if$copy$ptrcell$0x00,
    // lang$if$copy$cell$0x01,
    // lang$if$copy$ptrcell$0x01,
    // lang$if$copy$cell$0x02,
    // lang$if$copy$ptrcell$0x02,
    // lang$if$copy$cell$0x03,
    // lang$if$copy$ptrcell$0x03,

    // // copy to
    lang$if$copy$0x00$cell,
    // lang$if$copy$0x00$ptrcell,
    // lang$if$copy$0x01$cell,
    // lang$if$copy$0x01$ptrcell,
    // lang$if$copy$0x02$cell,
    // lang$if$copy$0x02$ptrcell,
    // lang$if$copy$0x03$cell,
    // lang$if$copy$0x03$ptrcell,

    // //// arithmetic
    // // 0x00
    lang$if$add$0x00$arg,
    lang$if$add$0x00$cell,
    // lang$if$add$0x00$ptrcell,
    lang$if$sub$0x00$arg,
    lang$if$sub$0x00$cell,
    // lang$if$sub$0x00$ptrcell,
    // lang$if$mul$0x00$arg,
    lang$if$mul$0x00$cell,
    // lang$if$mul$0x00$ptrcell,
    // lang$if$div$0x00$arg,
    lang$if$div$0x00$cell,
    // lang$if$div$0x00$ptrcell,
    // // 0x01
    // lang$if$add$0x01$arg,
    // lang$if$add$0x01$cell,
    // lang$if$add$0x01$ptrcell,
    // lang$if$sub$0x01$arg,
    // lang$if$sub$0x01$cell,
    // lang$if$sub$0x01$ptrcell,
    // lang$if$mul$0x01$arg,
    // lang$if$mul$0x01$cell,
    // lang$if$mul$0x01$ptrcell,
    // lang$if$div$0x01$arg,
    // lang$if$div$0x01$cell,
    // lang$if$div$0x01$ptrcell,
    // // 0x02
    // lang$if$add$0x02$arg,
    // lang$if$add$0x02$cell,
    // lang$if$add$0x02$ptrcell,
    // lang$if$sub$0x02$arg,
    // lang$if$sub$0x02$cell,
    // lang$if$sub$0x02$ptrcell,
    // lang$if$mul$0x02$arg,
    // lang$if$mul$0x02$cell,
    // lang$if$mul$0x02$ptrcell,
    // lang$if$div$0x02$arg,
    // lang$if$div$0x02$cell,
    // lang$if$div$0x02$ptrcell,
    // // 0x03
    // lang$if$add$0x03$arg,
    // lang$if$add$0x03$cell,
    // lang$if$add$0x03$ptrcell,
    // lang$if$sub$0x03$arg,
    // lang$if$sub$0x03$cell,
    // lang$if$sub$0x03$ptrcell,
    // lang$if$mul$0x03$arg,
    // lang$if$mul$0x03$cell,
    // lang$if$mul$0x03$ptrcell,
    // lang$if$div$0x03$arg,
    // lang$if$div$0x03$cell,
    // lang$if$div$0x03$ptrcell,

    // // if
    lang$if$if$0x00$skipinst$arg,
    // lang$if$if$0x00$skipinst$cell,
    // lang$if$if$0x00$skipinst$ptrcell,
    // lang$if$if$0x01$skipinst$arg,
    // lang$if$if$0x01$skipinst$cell,
    // lang$if$if$0x01$skipinst$ptrcell,
    // lang$if$if$0x02$skipinst$arg,
    // lang$if$if$0x02$skipinst$cell,
    // lang$if$if$0x02$skipinst$ptrcell,
    // lang$if$if$0x03$skipinst$arg,
    // lang$if$if$0x03$skipinst$cell,
    // lang$if$if$0x03$skipinst$ptrcell,

};

typedef enum{

    // output
    lang$ic$out$arg,
    lang$ic$out$cell,
    // lang$ic$out$ptrcell,

    // // input
    lang$ic$in$cell,
    // lang$ic$in$ptrcell,

    // // copy from
    lang$ic$copy$cell$0x00,
    // lang$ic$copy$ptrcell$0x00,
    // lang$ic$copy$cell$0x01,
    // lang$ic$copy$ptrcell$0x01,
    // lang$ic$copy$cell$0x02,
    // lang$ic$copy$ptrcell$0x02,
    // lang$ic$copy$cell$0x03,
    // lang$ic$copy$ptrcell$0x03,

    // // copy to
    lang$ic$copy$0x00$cell,
    // lang$ic$copy$0x00$ptrcell,
    // lang$ic$copy$0x01$cell,
    // lang$ic$copy$0x01$ptrcell,
    // lang$ic$copy$0x02$cell,
    // lang$ic$copy$0x02$ptrcell,
    // lang$ic$copy$0x03$cell,
    // lang$ic$copy$0x03$ptrcell,

    // //// arithmetic
    // // 0x00
    lang$ic$add$0x00$arg,
    lang$ic$add$0x00$cell,
    // lang$ic$add$0x00$ptrcell,
    lang$ic$sub$0x00$arg,
    lang$ic$sub$0x00$cell,
    // lang$ic$sub$0x00$ptrcell,
    // lang$ic$mul$0x00$arg,
    lang$ic$mul$0x00$cell,
    // lang$ic$mul$0x00$ptrcell,
    // lang$ic$div$0x00$arg,
    lang$ic$div$0x00$cell,
    // lang$ic$div$0x00$ptrcell,
    // // 0x01
    // lang$ic$add$0x01$arg,
    // lang$ic$add$0x01$cell,
    // lang$ic$add$0x01$ptrcell,
    // lang$ic$sub$0x01$arg,
    // lang$ic$sub$0x01$cell,
    // lang$ic$sub$0x01$ptrcell,
    // lang$ic$mul$0x01$arg,
    // lang$ic$mul$0x01$cell,
    // lang$ic$mul$0x01$ptrcell,
    // lang$ic$div$0x01$arg,
    // lang$ic$div$0x01$cell,
    // lang$ic$div$0x01$ptrcell,
    // // 0x02
    // lang$ic$add$0x02$arg,
    // lang$ic$add$0x02$cell,
    // lang$ic$add$0x02$ptrcell,
    // lang$ic$sub$0x02$arg,
    // lang$ic$sub$0x02$cell,
    // lang$ic$sub$0x02$ptrcell,
    // lang$ic$mul$0x02$arg,
    // lang$ic$mul$0x02$cell,
    // lang$ic$mul$0x02$ptrcell,
    // lang$ic$div$0x02$arg,
    // lang$ic$div$0x02$cell,
    // lang$ic$div$0x02$ptrcell,
    // // 0x03
    // lang$ic$add$0x03$arg,
    // lang$ic$add$0x03$cell,
    // lang$ic$add$0x03$ptrcell,
    // lang$ic$sub$0x03$arg,
    // lang$ic$sub$0x03$cell,
    // lang$ic$sub$0x03$ptrcell,
    // lang$ic$mul$0x03$arg,
    // lang$ic$mul$0x03$cell,
    // lang$ic$mul$0x03$ptrcell,
    // lang$ic$div$0x03$arg,
    // lang$ic$div$0x03$cell,
    // lang$ic$div$0x03$ptrcell,

    // // if
    lang$ic$if$0x00$skipinst$arg,
    // lang$ic$if$0x00$skipinst$cell,
    // lang$ic$if$0x00$skipinst$ptrcell,
    // lang$ic$if$0x01$skipinst$arg,
    // lang$ic$if$0x01$skipinst$cell,
    // lang$ic$if$0x01$skipinst$ptrcell,
    // lang$ic$if$0x02$skipinst$arg,
    // lang$ic$if$0x02$skipinst$cell,
    // lang$ic$if$0x02$skipinst$ptrcell,
    // lang$ic$if$0x03$skipinst$arg,
    // lang$ic$if$0x03$skipinst$cell,
    // lang$ic$if$0x03$skipinst$ptrcell,

    lang$ic$len,

}lang$instruction_code_t;
