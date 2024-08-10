
#define lang$program_data_t$init_from_cstr$MAX_NUMBER_OF_VARS 10

#define lang$CHECK_NARGS_OR_ERR(required_nargs, actual_nargs){ \
    if(required_nargs != actual_nargs){ \
        out$cstr("bad number of arguments: required=`"); \
        out$size(required_nargs); \
        out$cstr("` actual=`"); \
        out$size(actual_nargs); \
        out$cstr("`\n"); \
        return err$err; \
    } \
}

#define lang$CHECK_TYPE_SAME_OR_ERR(base_type, comparing_type){ \
    if(base_type != comparing_type){ \
        out$cstr("type missmatch: base=`"); \
        lang$print_type(base_type); \
        out$cstr("` comaring=`"); \
        lang$print_type(comparing_type); \
        out$cstr("`: comparing needs to be the same as base\n"); \
        return err$err; \
    } \
}

#define lang$CHECK_TYPE_SAME_OR_UNDECIDED_OR_ERR(base_type, comparing_type){ \
    if((comparing_type != lang$VT_UNDECIDED) && (base_type != comparing_type)){ \
        out$cstr("type missmatch: base=`"); \
        lang$print_type(base_type); \
        out$cstr("` comaring=`"); \
        lang$print_type(comparing_type); \
        out$cstr("`: comparing needs to be the same as base or undecided\n"); \
        return err$err; \
    } \
}

#define lang$CHECK_TYPE_PTR_OR_ERR(type){ \
    if(!lang$is_ptr_type(type)){ \
        out$cstr("pointer type required, instead got `"); \
        lang$print_type(type); \
        out$cstr("`\n"); \
        return err$err; \
    } \
}

typedef enum{
    lang$VT_UNDECIDED,
    lang$VT_CHAR,
    lang$VT_PTR_CHAR,
}lang$var_type_t;

void lang$print_type(lang$var_type_t type){
    switch(type){
        case lang$VT_UNDECIDED:
            out$cstr("undecided");
            break;
        case lang$VT_CHAR:
            out$cstr("char");
            break;
        case lang$VT_PTR_CHAR:
            out$cstr("ptr-char");
            break;
    }
}

bool lang$is_ptr_type(lang$var_type_t type){
    switch(type){
        case lang$VT_UNDECIDED:
        case lang$VT_CHAR:
            return false;
        case lang$VT_PTR_CHAR:
            return true;
    }
    out$cstr("unreachable\n");
    return false; // unreachable
}

err_t lang$ptr_type_of(lang$var_type_t type, lang$var_type_t * out){
    switch(type){
        case lang$VT_UNDECIDED:
            return err$err;
        case lang$VT_CHAR:
            * out = lang$VT_PTR_CHAR;
            return err$ok;
        case lang$VT_PTR_CHAR:
            return err$err;
    }
    out$cstr("unreachable\n");
    return err$err; // unreachable
}

err_t lang$strlen_to_type(char * str, size_t str_len, lang$var_type_t * type){
    if(strlen_sameas_cstr(str, str_len, "undecided")){
        * type = lang$VT_UNDECIDED;
    }else if(strlen_sameas_cstr(str, str_len, "char")){
        * type = lang$VT_CHAR;
    }else if(strlen_sameas_cstr(str, str_len, "ptr-char")){
        * type = lang$VT_PTR_CHAR;
    }else{
        out$cstr("unknown type `");
        out$strlen(str, str_len);
        out$cstr("`\n");
        return err$err;
    }

    return err$ok;
}

typedef struct{
    char name[lang$init_from_cstr$WORD_MAXLEN];
    size_t name_len;

    uint8_t addr;

    lang$var_type_t type;
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

err_t lang$compiler_t$add_var(
    lang$compiler_t * ctx,
    char * name, size_t name_len,
    lang$var_type_t type
){

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
    ctx->vars[ctx->vars_len].type = type;

    ctx->vars_len += 1;
    ctx->next_var_addr += 1;

    return err$ok;
}

err_t lang$compiler_t$find_var(
    lang$compiler_t * ctx,
    char * name, size_t name_len,
    uint8_t * out_addr, lang$var_type_t * * out_type
){

    for(size_t idx=0; idx<ctx->vars_len; ++idx){
        if(strlen_sameas_strlen(name, name_len, ctx->vars[idx].name, ctx->vars[idx].name_len)){
            * out_addr = ctx->vars[idx].addr;
            * out_type = & ctx->vars[idx].type;
            return err$ok;
        }
    }

    return err$err;

}

err_t lang$compiler_t$get_arg_value(
    lang$compiler_t * ctx,
    char * arg, size_t arg_len,
    uint8_t * out_value, lang$var_type_t * out_type
){
    {
        err_t err = strlen_to_u8(arg, arg_len, out_value);
        if(!err){
            * out_type = lang$VT_UNDECIDED;
            return err$ok;
        }
    }

    lang$var_type_t * addr_variable_type;
    err_t err = lang$compiler_t$find_var(ctx, arg, arg_len, out_value, &addr_variable_type);

    if(err){
        out$cstr("could not determine value of argument `");
        out$strlen(arg, arg_len);
        out$cstr("`\n");
        return err$err;
    }

    * out_type = * addr_variable_type;

    return err$ok;
}

err_t lang$compiler_t$compile_instruction(
    lang$compiler_t * ctx,
    char * inst, size_t inst_len,
    char arguments[lang$init_from_cstr$INST_MAX_ARGS][lang$init_from_cstr$WORD_MAXLEN], size_t * argument_lens, size_t arguments_len, // the array is passed by reference
    bool * inst0_set, uint8_t * inst0, uint8_t * inst0_arg,
    bool * inst1_set, uint8_t * inst1, uint8_t * inst1_arg,
    bool * inst2_set, uint8_t * inst2, uint8_t * inst2_arg
){

    * inst0_set = false;
    * inst1_set = false;
    * inst2_set = false;

    // debug

    // out$cstr("[dbg: processing instruction `");
    // out$strlen(inst, inst_len);
    // out$cstr("`]\n");

    // compiler directive

    if(strlen_sameas_cstr(inst, inst_len, "var")){

        lang$CHECK_NARGS_OR_ERR(1, arguments_len);
        return lang$compiler_t$add_var(ctx, arguments[0], argument_lens[0], lang$VT_UNDECIDED);

    }else if(strlen_sameas_cstr(inst, inst_len, "cast")){

        lang$CHECK_NARGS_OR_ERR(2, arguments_len);

        uint8_t var_addr;
        lang$var_type_t * addr_var_type;
        {
            err_t err = lang$compiler_t$find_var(ctx, arguments[0], argument_lens[0], &var_addr, &addr_var_type);
            if(err){
                out$cstr("no such variable exists `");
                out$strlen(arguments[0], argument_lens[0]);
                out$cstr("`\n");
                return err$err;
            }
        }

        lang$var_type_t cast_to_regular;
        if(lang$strlen_to_type(arguments[1], argument_lens[1], &cast_to_regular)){
            return err$err;
        }

        lang$var_type_t cast_to_pointer;
        if(lang$ptr_type_of(cast_to_regular, &cast_to_pointer)){
            out$cstr("internal error related to ptrs\n");
            return err$err;
        }

        * addr_var_type = cast_to_pointer;

        return err$ok;

    }

    // modify variable value
    // note that this is going to fuck you over if some
    // retard has overwritten certain addresses as variable names

    do{
        uint8_t var_addr;
        lang$var_type_t * addr_var_type;
        {
            err_t err = lang$compiler_t$find_var(ctx, inst, inst_len, &var_addr, &addr_var_type);
            if(err){
                break;
            }
        }

        lang$CHECK_NARGS_OR_ERR(2, arguments_len);

        if(strlen_sameas_cstr(arguments[0], argument_lens[0], "=")){

            // special assignment case

            if(strlen_sameas_cstr(arguments[1], argument_lens[1], "$getchar")){
                lang$CHECK_TYPE_SAME_OR_UNDECIDED_OR_ERR(lang$VT_PTR_CHAR, *addr_var_type);
                * addr_var_type = lang$VT_PTR_CHAR;
                * inst0_set = true;
                * inst0 = lang$ic$in$cell;
                * inst0_arg = var_addr;
                return err$ok;
            }

            // regular assignment case

            uint8_t arg_value;
            lang$var_type_t arg_type;
            err_t err = lang$compiler_t$get_arg_value(ctx, arguments[1], argument_lens[1], &arg_value, &arg_type);
            if(err){
                return err;
            }

            lang$CHECK_TYPE_SAME_OR_UNDECIDED_OR_ERR(*addr_var_type, arg_type);
    
            * inst0_set = true;
            * inst0 = lang$ic$copy$arg$0x00;
            * inst0_arg = arg_value;

            * inst1_set = true;
            * inst1 = lang$ic$copy$0x00$cell;
            * inst1_arg = var_addr;

            return err$ok;

        }

        uint8_t arg_value;
        lang$var_type_t arg_type;
        err_t err = lang$compiler_t$get_arg_value(ctx, arguments[1], argument_lens[1], &arg_value, &arg_type);
        if(err){
            return err;
        }

        if(strlen_sameas_cstr(arguments[0], argument_lens[0], "+cell=")){ // if we had types system we could ommit the stupid "cell"

            lang$CHECK_TYPE_PTR_OR_ERR(*addr_var_type);
            lang$CHECK_TYPE_SAME_OR_ERR(*addr_var_type, arg_type);

            * inst0_set = true;
            * inst0 = lang$ic$copy$cell$0x00;
            * inst0_arg = var_addr;

            * inst1_set = true;
            * inst1 = lang$ic$add$0x00$cell;
            * inst1_arg = arg_value;

            * inst2_set = true;
            * inst2 = lang$ic$copy$0x00$cell;
            * inst2_arg = var_addr;

            return err$ok;

        }else if(strlen_sameas_cstr(arguments[0], argument_lens[0], "-cell=")){
            
            lang$CHECK_TYPE_PTR_OR_ERR(*addr_var_type);
            lang$CHECK_TYPE_SAME_OR_ERR(*addr_var_type, arg_type);

            * inst0_set = true;
            * inst0 = lang$ic$copy$cell$0x00;
            * inst0_arg = var_addr;

            * inst1_set = true;
            * inst1 = lang$ic$sub$0x00$cell;
            * inst1_arg = arg_value;

            * inst2_set = true;
            * inst2 = lang$ic$copy$0x00$cell;
            * inst2_arg = var_addr;

            return err$ok;

        }

        out$cstr("unknown operator `");
        out$strlen(arguments[0], argument_lens[0]);
        out$cstr("`\n");
        return err$err;

    }while(false);

    // actual instruction

    lang$CHECK_NARGS_OR_ERR(1, arguments_len);

    uint8_t arg_value;
    lang$var_type_t arg_type;
    err_t err = lang$compiler_t$get_arg_value(ctx, arguments[0], argument_lens[0], &arg_value, &arg_type);
    if(err){
        return err;
    }

    * inst0_arg = arg_value;

    if(strlen_sameas_cstr(inst, inst_len, "out$arg")){
        // will print anything, no matter what type
        * inst0_set = true;
        * inst0 = lang$ic$out$arg;
        return err$ok;
    }else if(strlen_sameas_cstr(inst, inst_len, "out$cell")){
        lang$CHECK_TYPE_PTR_OR_ERR(arg_type);
        * inst0_set = true;
        * inst0 = lang$ic$out$cell;
        return err$ok;
    }
    // unknown

    out$cstr("unknown instruction `");
    out$strlen(inst, inst_len);
    out$cstr("`\n");

    return err$err;
}
