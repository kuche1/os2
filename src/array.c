
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

// no need to make things more complicated by
// indicating if the arr was already empty
#define arr$$GENERATE_FNC_DEL_LAST(type) \
    void arr$ ## type ## $del_last(arr_ ## type * arr){ \
        if(arr->len <= 0){ \
            return; \
        } \
        arr->len -= 1; \
    }

#define arr$$GENERATE_ALL(type) \
    arr$$GENERATE_STRUCT(type) \
    arr$$GENERATE_FNC_PUSH(type) \
    arr$$GENERATE_FNC_DEL_LAST(type)

///
////// typedef missing types
///

// typedef the types that don't end with `_t`
// so that we can make arrays of them
typedef char char_t;

///
////// init
///

#define arr$INIT(type, var_name, capacity) \
    type ___ ## var_name ## _data [capacity]; \
    arr_ ## type ___ ## var_name ## _struct = { \
        .data = ___ ## var_name ## _data, \
        .len = 0, \
        .cap = capacity, \
    }; \
    arr_ ## type * var_name = & ___ ## var_name ## _struct;

///
////// char_t
///

// // typedef struct{
// //     char * data;
// //     size_t len;
// //     size_t cap;
// // }arr_char_t;
// arr$$GENERATE_STRUCT(char_t)

// // err_t arr$char_t$push(arr_char_t * arr, char data){
// //     if(arr->len >= arr->cap){
// //         return true;
// //     }
// //     arr->data[arr->len++] = data;
// //     return false;
// // }
// arr$$GENERATE_FNC_PUSH(char_t)

// // void arr$char_t$del_last(arr_char_t * arr){
// //     if(arr->len <= 0){
// //         return;
// //     }
// //     arr->len -= 1;
// // }
// arr$$GENERATE_FNC_DEL_LAST(char_t)

arr$$GENERATE_ALL(char_t)

err_t arr$char_t$pop_head(arr_char_t * arr, char * out_popped_item){
    if(arr->len <= 0){
        return err$ERR;
    }
    
    char ch = arr->data[0];

    for(size_t i=0; i+1<arr->len; ++i){
        arr->data[i] = arr->data[i+1];
    }

    arr->len -= 1;

    * out_popped_item = ch;
    return err$OK;
}

bool arr$char_t$same_as$cstr(const arr_char_t * arr, const char * cstr){

    size_t idx = 0;

    for(;; idx+=1){

        char c_ch = cstr[idx];

        bool last_c = c_ch == 0;
        bool last_arr = arr->len == idx;

        if(last_c != last_arr){
            // one is done and the other is not
            return false;
        }

        if(last_c){
            // they're both done
            return true;
        }

        // they both are not done

        char arr_ch = arr->data[idx];

        if(c_ch != arr_ch){
            return false;
        }

    }

}

///
////// arr_char_t
///

// // I don't know if this actually works

// arr$$GENERATE_ALL(arr_char_t)

// #define arr$arr_char_t$push$CSTR(arr, cstr)
//     arr$INIT(char_t, ___ ## arr ## _cstr_data_ ## __LINE__, )

// bool arr$arr_char_t$push$cstr(arr_arr_char_t * arr, char * data){
//     ...
// }
