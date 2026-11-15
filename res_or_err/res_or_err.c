#include "res_or_err.h"

union res_union unpack(struct result_or_error r, void (*on_error)(int, void *), void *context) {
    if (r.ok) {
        return r.result;
    } else {
        on_error(r.error, context);
        union res_union nul;
        nul.as_ullong = 0;
        return nul;
    }
}

unsigned long long ullong_unpack(struct result_or_error r, void (*on_error)(int, void *), void *context) {
    return unpack(r, on_error, context).as_ullong;
}

long long llong_unpack(struct result_or_error r, void (*on_error)(int, void *), void *context) {
    return unpack(r, on_error, context).as_llong;
}

double dfloat_unpack(struct result_or_error r, void (*on_error)(int, void *), void *context) {
    return unpack(r, on_error, context).as_double;
}

char char_unpack(struct result_or_error r, void (*on_error)(int, void *), void *context) {
    return unpack(r, on_error, context).as_char;
}

void *ptr_unpack(struct result_or_error r, void (*on_error)(int, void *), void *context) {
    return unpack(r, on_error, context).as_ptr;
}
