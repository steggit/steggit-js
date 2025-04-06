#ifndef WRAPPER_UTILS_H
#define WRAPPER_UTILS_H

#include "types.h"
#include <node_api.h>
typedef struct {
  napi_deferred deferred;
  napi_async_work work;
  Config config;
  char *error;
  char *output;
  int result;
} PromiseData;

int parse_napi_args(napi_env env, napi_callback_info info, Config *config);

void free_config(Config *config);

void handle_encode_error(napi_env env, PromiseData *promise_data,
                         const char *error_message);

void handle_decode_error(napi_env env, PromiseData *promise_data,
                         const char *error_message);

#endif // WRAPPER_UTILS_H