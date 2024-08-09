
#define lang$program_data_t$init_from_cstr$MAX_NUMBER_OF_VARS 10
// TODO increase after it has been tested

typedef struct{
    char name[lang$init_from_cstr$WORD_MAXLEN];
    size_t name_len;

    uint8_t addr;
}lang$varnamelen_addr_t;

typedef struct{
    lang$varnamelen_addr_t vars[lang$program_data_t$init_from_cstr$MAX_NUMBER_OF_VARS];
    size_t vars_len;

    uint8_t next_var_addr;
}lang$compiler_t;

void lang$compiler_t$init(lang$compiler_t * ctx){
    ctx->vars_len = 0;
    ctx->next_var_addr = 0x10;
}

err_t lang$compiler_t$add_var(lang$compiler_t * ctx, char * name, size_t name_len){

    for(size_t idx=0; idx<ctx->vars_len; ++idx){
        if(strlen_sameas_strlen(name, name_len, ctx->vars[idx].name, ctx->vars[idx].name_len)){
            out$cstr("variable already exists `");
            out$strlen(name, name_len);
            out$cstr("`\n");
            return err$err;
        }
    }

    if(ctx->vars_len >= LENOF(ctx->vars)){
        out$cstr("not enough space for new var `");
        out$strlen(name, name_len);
        out$cstr("`\n");
        return err$err;
    }

    {
        err_t err = copy(name, name_len, ctx->vars[ctx->vars_len].name, sizeof(ctx->vars[ctx->vars_len].name), name_len);
        if(err){
            out$cstr("could not copy variable name `");
            out$strlen(name, name_len);
            out$cstr("`\n");
            return err$err;
        }
    }
    ctx->vars[ctx->vars_len].name_len = name_len;
    ctx->vars[ctx->vars_len].addr = ctx->next_var_addr;

    // out$cstr("[dbg: set addr of `");
    // out$strlen(name, name_len);
    // out$cstr("`");

    // out$cstr(" also known as `");
    // out$strlen(ctx->vars[ctx->vars_len].name, ctx->vars[ctx->vars_len].name_len);
    // out$cstr("`");

    // out$cstr(" to `");
    // out$u8(ctx->next_var_addr);
    // out$cstr("`");

    // out$cstr(" also known as `");
    // out$u8(ctx->vars[ctx->vars_len].addr);
    // out$cstr("`]");

    ctx->vars_len += 1;
    ctx->next_var_addr += 1;

    return err$ok;
}

err_t lang$compiler_t$find_var(lang$compiler_t * ctx, char * name, size_t name_len, uint8_t * out_var_addr){

    for(size_t idx=0; idx<ctx->vars_len; ++idx){
        if(strlen_sameas_strlen(name, name_len, ctx->vars[idx].name, ctx->vars[idx].name_len)){
            * out_var_addr = ctx->vars[idx].addr;
            return err$ok;
        }
    }

    return err$err;

}

err_t lang$compiler_t$process_directive(
    lang$compiler_t * ctx, char * inst, size_t inst_len, char * arg, size_t arg_len,
    bool * out_compiler_directive_processed
){

    * out_compiler_directive_processed = false;
    
    if(strlen_sameas_cstr(inst, inst_len, "var")){
        * out_compiler_directive_processed = true;
        return lang$compiler_t$add_var(ctx, arg, arg_len);
    }

    * out_compiler_directive_processed = false;
    return err$ok;
}
