
[[nodiscard]] err_t lang$delete_instruction_and_arg_at(lang$program_data_t * ctx, size_t idx){
    for(size_t copy_target_idx=idx;; ++copy_target_idx){

        size_t copy_source_idx = copy_target_idx + lang$INSTRUCTION_SIZE;
        if(copy_source_idx >= ctx->code_len){
            break;
        }
        ctx->code[copy_target_idx] = ctx->code[copy_source_idx];

    }

    if(ctx->code_len < lang$INSTRUCTION_SIZE){
        out$cstr("attempt to delete instruction while queue is empty\n");
        return err$err;
    }

    ctx->code_len -= lang$INSTRUCTION_SIZE;

    return err$ok;
}

[[nodiscard]] err_t lang$process_special_instructions(lang$program_data_t * ctx){
    
    // TODO we don't have to do this after all has been finished
    // we could do it when we find `}` right away, so that it's easier
    // to get the correct line in the error message

    size_t open_code_blocks = 0;

    for(size_t inst_idx=0; inst_idx<ctx->code_len; inst_idx+=lang$INSTRUCTION_SIZE){
        uint8_t inst = ctx->code[inst_idx];

        if(inst == lang$ic$code_block_begin){
            open_code_blocks += 1;
        }

        if(inst != lang$ic$code_block_end){
            continue;
        }

        open_code_blocks -= 1;

        for(size_t find_open_idx=inst_idx;; find_open_idx-=lang$INSTRUCTION_SIZE){
            uint8_t find_open_inst = ctx->code[find_open_idx];

            if(find_open_inst == lang$ic$code_block_begin){

                // change the argument of the previous instruction to the difference of the open and closed brackets in number of instructions

                size_t open_closed_bracket_diff_idx_size = inst_idx - find_open_idx;
                open_closed_bracket_diff_idx_size -= lang$INSTRUCTION_SIZE; // take into account the open bracket

                ASSERT(open_closed_bracket_diff_idx_size % lang$INSTRUCTION_SIZE == 0, "f");
                size_t ocb_diff_inst = open_closed_bracket_diff_idx_size / lang$INSTRUCTION_SIZE;

                // out$cstr("dbg: open bracket is at idx=");
                // out$size(find_open_idx);
                // out$cstr(" closed bracket is at idx=");
                // out$size(inst_idx);
                // out$cstr(" and diff=");
                // out$size(open_closed_bracket_diff_size);
                // out$cstr("\n");

                ASSERT(ocb_diff_inst <= 256, "the open and closed code brackets are too far away"); // 256 - uint8_t limit
                uint8_t open_closed_bracket_diff = (uint8_t) ocb_diff_inst;

                ASSERT(find_open_idx > 0, "b");
                ctx->code[find_open_idx-1] = open_closed_bracket_diff; // this is the previous instruction argument

                // delete closed brackets
                err$CHECK( lang$delete_instruction_and_arg_at(ctx, inst_idx), "(c)");

                // delete open brackets
                err$CHECK( lang$delete_instruction_and_arg_at(ctx, find_open_idx), "(d)");

                // compensate for deletions
                inst_idx -= 2 * lang$INSTRUCTION_SIZE;

                break;
            }

            if(find_open_idx <= 0){
                out$cstr("too many code block ends, not enough code block begins\n");
                return err$err;
            }

        }

    }

    if(open_code_blocks > 0){
        out$cstr("too many open code blocks\n");
        return err$err;
    }

    return err$ok;
}

err_t lang$program_data_t$init_from_instruction_code(lang$program_data_t * ctx, uint8_t * code, size_t code_len){
    for(size_t i=0; i<CAPOF(ctx->mem); ++i){
        ctx->mem[i] = 0;
    }

    ctx->code = code;
    ctx->code_len = code_len;
    if(code_len % lang$INSTRUCTION_SIZE != 0){
        out$cstr("bad code length\n");
        return err$err;
    }

    ctx->instruction_index = 0;

    if( lang$process_special_instructions(ctx) ){
        return err$err;
    }

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

        if(inst >= CAPOF(lang$instruction_lookup)){
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
