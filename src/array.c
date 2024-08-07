
///
////// struct/fnc generators
///

#define arr$$GENERATE_STRUCT(type) \
    typedef struct{ \
        type * data; \
        size_t len; \
        size_t cap; \
    } arr_ ## type;

#define arr$$GENERATE_FNC_PUSH(type) \
    err_t arr$ ## type ## $push(arr_ ## type * arr, type data){ \
        if(arr->len >= arr->cap){ \
            return err$ERR; \
        } \
        arr->data[arr->len++] = data; \
        return err$OK; \
    }

///
////// typedef missing types
///

// typedef the types that don't end with `_t`
// so that we can make arrays of them
typedef char char_t;

///
////// ...
///

#define arr$INIT(type, var_name, capacity) \
    type ___ ## var_name ## _data [capacity]; \
    arr_ ## type ___ ## var_name ## _struct = { \
        .data = ___ ## var_name ## _data, \
        .len = 0, \
        .cap = capacity, \
    }; \
    arr_ ## type * var_name = & ___ ## var_name ## _struct;

// typedef struct{
//     char * data;
//     size_t len;
//     size_t cap;
// }arr_char_t;
arr$$GENERATE_STRUCT(char_t)

// err_t arr$char_t$push(arr_char_t * arr, char data){
//     if(arr->len >= arr->cap){
//         return true;
//     }
//     arr->data[arr->len++] = data;
//     return false;
// }
arr$$GENERATE_FNC_PUSH(char_t)

// no need to make things more complicated by
// indicating if the arr was already empty
void arr$char_t$del_last(arr_char_t * arr){
    if(arr->len <= 0){
        return;
    }
    arr->len -= 1;
}

bool arr$char_t$same_as$cstr(const arr_char_t * arr, const char * cstr){

    size_t idx = 0;

    for(;; idx+=1){

        char c_ch = cstr[idx];

        bool last_c = c_ch == 0;
        bool last_arr = arr->len == idx;

        if(last_c != last_arr){
            // one is done and the other is not
            // out$cstr("[dl]");
            return false;
        }

        if(last_c){
            // they're both done
            return true;
        }

        // they both are not done

        char arr_ch = arr->data[idx];

        if(c_ch != arr_ch){
            // out$cstr("[dc");
            // out$cstr("<");
            // out$ch(c_ch);
            // out$ch(arr_ch);
            // out$cstr(">");
            // out$cstr("]");
            return false;
        }

    }

}
