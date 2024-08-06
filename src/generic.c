
///
////// error
///

typedef bool err_t;

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

///
////// array
///

#define arr$INIT(type, var_name, capacity) \
    type ___ ## var_name ## _data [capacity]; \
    arr_ ## type ## _t ___ ## var_name ## _struct = { \
        .data = ___ ## var_name ## _data, \
        .len = 0, \
        .cap = capacity, \
    }; \
    arr_ ## type ## _t * var_name = & ___ ## var_name ## _struct;

typedef struct{
    char * data;
    size_t len;
    size_t cap;
}arr_char_t;

err_t arr$char$push(arr_char_t * arr, char data){
    if(arr->len >= arr->cap){
        return true;
    }
    arr->data[arr->len++] = data;
    return false;
}

// no need to make things more complicated by
// indicating if the arr was already empty
void arr$char$del_last(arr_char_t * arr){
    if(arr->len <= 0){
        return;
    }
    arr->len -= 1;
}

bool arr$char$same_as$cstr(arr_char_t * arr, char * cstr){

    size_t idx = 0;

    for(;; idx+=1){

        char c_ch = cstr[idx];

        bool last_c = c_ch == 0;
        bool last_arr = arr->len == idx;

        if(last_c != last_arr){
            return false;
        }

        char arr_ch = arr->data[idx];

        if(c_ch != arr_ch){
            return false;
        }

        if(last_c && last_arr){
            return true;
        }
    }

}

// TODO del_char

// ///
// ////// string
// ///

// typedef struct{
//     char * data;
//     size_t len;
// }str_t;
