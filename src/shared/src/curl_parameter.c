
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/curl_frontend.h>
#include <fightable/string.h>

RSB_ARRAY_IMPL_GEN(struct fcurl_parameter, CurlParameter);

struct fcurl_parameter _fCurlParameterCreateWithString(const char *key, const char *value) {
    struct fcurl_parameter param = {};

    param.key = _fCopyString(key);
    param.value = _fCopyString(value);

    return param;
}
struct fcurl_parameter _fCurlParameterCreateWithInteger(const char *key, int value) {
    char buf[33] = {};
    snprintf(buf, 32, "%d", value);

    return _fCurlParameterCreateWithString(key, buf);
}
void _fCurlParameterDestroy(struct fcurl_parameter *p) {
    if (!p) return;

    if (p->key) {
        free(p->key);
        p->key = NULL;
    }
    if (p->value) {
        free(p->value);
        p->value = NULL;
    }
}
