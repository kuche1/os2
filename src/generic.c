
///
////// error
///

typedef bool err_t;

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
////// ...
///

#define LENOF(arr) \
    ( sizeof(arr) / sizeof(*arr) )
