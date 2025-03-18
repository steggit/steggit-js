#ifndef WRAPPER_UTILS_H
#define WRAPPER_UTILS_H

#include "types.h"
#include <node_api.h>

int parse_napi_args(napi_env env, napi_callback_info info, Config *config);

#endif // WRAPPER_UTILS_H