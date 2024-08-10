
///
////// assert
///

// doesn't work in global scope
#define COMPTIME_ASSERT(cond) \
    ( (void) sizeof(char[1 - 2*!(cond)]) )

///
////// lenof
///

// TODO rename to CAPOF
#define LENOF(arr) \
    ( sizeof(arr) / sizeof(*arr) )


///
////// error
///

// TODO since we have standardised this, we can now make
// some macro that prints and error message and returns
// err$err

// TODO we need to find a way to declare all err_t functions
// with [[nodiscard]]

typedef bool err_t;

#define err$ok false
#define err$err true

#define err$CHECK(fnc, err_msg) \
    if((fnc) == err$err){ \
        out$cstr(err_msg); \
        out$nl(); \
        return err$err; \
    }

#define UNREACHABLE(identifier) \
    out$cstr("unreachable code reached ("); \
    out$cstr(identifier); \
    out$cstr(")\n"); \
    return err$err;

#define ASSERT(cond, identifier) \
    if(!(cond)){ \
        out$cstr("assertion failed ("); \
        out$cstr(identifier); \
        out$cstr(")\n"); \
        return err$err; \
    }

///
////// copy
///

[[nodiscard]] err_t copy(void * src, size_t src_size, void * dst, size_t dst_size, size_t number_of_bytes_to_copy){

    if(src_size < number_of_bytes_to_copy){
        return err$err;
    }

    if(dst_size < number_of_bytes_to_copy){
        return err$err;
    }

    uint8_t * dst_u8 = dst;
    uint8_t * src_u8 = src;
    for(size_t i=0; i<number_of_bytes_to_copy; ++i){
        dst_u8[i] = src_u8[i];
    }

    return err$ok;
}

///
////// array
///

#include "array.c"

///
////// string
///

bool strlen_sameas_strlen(char * str0, size_t str0_len, char * str1, size_t str1_len){

    if(str0_len != str1_len){
        return false;
    }

    for(size_t i=0; i<str0_len; ++i){

        char ch0 = str0[i];
        char ch1 = str1[i];

        if(ch0 != ch1){
            return false;
        }

    }

    return true;
}

bool strlen_sameas_cstr(char * cstrlen, size_t cstrlen_len, char * cstr){
    for(size_t i=0;; ++i){

        char ch1 = cstr[i];

        bool str0_finished = i == cstrlen_len;
        bool str1_finished = ch1 == 0;

        if(str0_finished && str1_finished){
            return true;
        }else if(str0_finished != str1_finished){
            return false;
        }

        char ch0 = cstrlen[i];

        if(ch0 != ch1){
            return false;
        }
    }

    return false; // this must be unreachable
}

err_t strlen_to_u8(char * arg, size_t arg_len, uint8_t * out_u8){

    uint8_t num = 0;

    for(size_t idx=0; idx<arg_len; ++idx){

        char ch = arg[idx];

        if((ch >= '0') && (ch <= '9')){
            num *= 10;
            num += (uint8_t) (ch - '0');
            continue;
        }

        return err$err;

    }

    * out_u8 = num;
    return err$ok;

}

bool strlen_startswith_cstr(char * strl, size_t strl_len, char * cstr){
    for(size_t idx=0; idx<strl_len; ++idx){
        char ch_strl = strl[idx];
        char ch_cstr = *cstr++;
        if(!ch_cstr){
            return true;
        }
        if(ch_strl != ch_cstr){
            return false;
        }
    }
    if(!(*cstr)){
        return true;
    }
    return false;
}
