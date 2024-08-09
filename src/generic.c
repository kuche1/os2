
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
////// copy
///

// TODO make another version of this that checks both var ranges
// keep in mind: this is called `copy` not `copy_partial`
void copy(const void * src, void * dst, size_t number_of_bytes_to_copy){
    uint8_t * dst_u8 = dst;
    const uint8_t * src_u8 = src;
    for(size_t i=0; i<number_of_bytes_to_copy; ++i){
        dst_u8[i] = src_u8[i];
    }
}

///
////// error
///

// TODO she gi eba teq laina ujasna ideq sa,
// trqbva si4kite da gi iztriq osven err_t

typedef bool err_t;

// TODO use lowercase
#define err$OK false
#define err$ERR true

// unpack `err_or_<type>_t`
#define UNP(var_err, type, var_data, fnc) \
    bool var_err; \
    type var_data; \
    { \
        err_or_ ## type ## _t err_or_data = fnc; \
        var_err = err_or_data.err; \
        var_data = err_or_data.data; \
    }

#define UNP_U8_U8(var_err, var_data0, var_data1, fnc) \
    bool var_err; \
    uint8_t var_data0; \
    uint8_t var_data1; \
    { \
        err_or_u8_u8_t err_or_data = fnc; \
        var_err = err_or_data.err; \
        var_data0 = err_or_data.data0; \
        var_data1 = err_or_data.data1; \
    }

typedef struct{
    err_t err;
    char data;
}err_or_char_t;

typedef struct{
    err_t err;
    bool data;
}err_or_bool_t;

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

        return err$ERR;

    }

    * out_u8 = num;
    return err$OK;

}
