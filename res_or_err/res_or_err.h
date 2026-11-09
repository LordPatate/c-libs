#ifndef RES_OR_ERR_H
#define RES_OR_ERR_H

enum res_or_err_type {
    ERROR_TYPE,
    RESULT_TYPE
};

struct result_or_error {
    enum res_or_err_type ok;
    union {
        void *result;
        int error;
    };
};

// Wrap CONTENT as a result in a struct result_or_error.
#define RESULT(CONTENT) (struct result_or_error) {RESULT_TYPE, .result = (CONTENT)}

// Wrap ERROR_CODE as an error in a struct result_or_error.
#define ERROR(ERROR_CODE) (struct result_or_error) {ERROR_TYPE, .error = (ERROR_CODE)}

// Try to unpack r.
// On success, return r.result.
// On failure, call on_error(r.error, context) if on_error is not NULL;
// then return a nul pointer.
void *unpack(struct result_or_error r, void (*on_error)(int, void *), void *context);

#endif // !RES_OR_ERR_H
