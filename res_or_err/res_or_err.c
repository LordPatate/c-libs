#include "res_or_err.h"

void *unpack(struct result_or_error r, void (*on_error)(int, void *), void *context) {
    if (r.ok) {
        return r.result;
    } else {
        on_error(r.error, context);
        return (void *) 0;
    }
}
