
///
////// error
///

// unpack `err_or_char_t`
#define UNP(var_err, type, var_data, fnc) \
    bool var_err; \
    type var_data; \
    { \
        err_or_ ## type ## _t err_or_data = fnc; \
        var_err = err_or_data.err; \
        var_data = err_or_data.data; \
    }

typedef struct{
    bool err;
    char data;
}err_or_char_t;

///
////// string
///

typedef struct{
    char * data;
    size_t len;
}str_t;
