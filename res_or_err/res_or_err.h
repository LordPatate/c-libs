#ifndef RES_OR_ERR_H
#define RES_OR_ERR_H

enum res_or_err_type {
    ERROR_TYPE,
    RESULT_TYPE
};

union res_union {
    unsigned long long int as_ullong;
    long long int as_llong;
    double as_double;
    char as_char;
    void *as_ptr;
};

struct result_or_error {
    enum res_or_err_type ok;
    union {
        int error;
        union res_union result;
    };
};

// Wrap CONTENT as a result of type TYPE in a struct result_or_error.
#define RESULT(CONTENT, TYPE) (struct result_or_error) {RESULT_TYPE, .result = {.as_##TYPE=(CONTENT)}}

// Wrap an unsigned long long int as a result in a struct result_or_error.
#define ULLONG_RESULT(CONTENT) RESULT(CONTENT, ullong)

// Wrap a long long int as a result in a struct result_or_error.
#define LLONG_RESULT(CONTENT) RESULT(CONTENT, llong)

// Wrap a double as a result in a struct result_or_error.
#define DOUBLE_RESULT(CONTENT) RESULT(CONTENT, double)

// Wrap a char as a result in a struct result_or_error.
#define CHAR_RESULT(CONTENT) RESULT(CONTENT, char)

// Wrap a pointer as a result in a struct result_or_error.
#define PTR_RESULT(CONTENT) RESULT(CONTENT, ptr)

// Wrap ERROR_CODE as an error in a struct result_or_error.
#define ERROR(ERROR_CODE) (struct result_or_error) {ERROR_TYPE, .error = (ERROR_CODE)}

// Try to unpack a result r.
// On success, return r.result.
// On failure, call on_error(r.error, context) if on_error is not NULL;
// then return a nul value.
union res_union unpack(struct result_or_error r, void (*on_error)(int, void *), void *context);

// Try to unpack a result r supposed to wrap an unsigned long long int.
// On success, return r.result.as_ullong.
// On failure, call on_error(r.error, context) if on_error is not NULL;
// then return 0.
unsigned long long ullong_unpack(struct result_or_error r, void (*on_error)(int, void *), void *context);

// Try to unpack a result r supposed to wrap a long long int.
// On success, return r.result.as_llong.
// On failure, call on_error(r.error, context) if on_error is not NULL;
// then return 0.
long long llong_unpack(struct result_or_error r, void (*on_error)(int, void *), void *context);

// Try to unpack a result r supposed to wrap a double.
// On success, return r.result.as_double.
// On failure, call on_error(r.error, context) if on_error is not NULL;
// then return 0.
double double_unpack(struct result_or_error r, void (*on_error)(int, void *), void *context);

// Try to unpack a result r supposed to wrap a char.
// On success, return r.result.as_char.
// On failure, call on_error(r.error, context) if on_error is not NULL;
// then return a nul pointer.
char char_unpack(struct result_or_error r, void (*on_error)(int, void *), void *context);

// Try to unpack a result r supposed to wrap a pointer.
// On success, return r.result.as_ptr.
// On failure, call on_error(r.error, context) if on_error is not NULL;
// then return a nul pointer.
void *ptr_unpack(struct result_or_error r, void (*on_error)(int, void *), void *context);

#endif // !RES_OR_ERR_H
