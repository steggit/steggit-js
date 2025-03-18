#define NAPI_VERSION 9
#include "general_utils.h"
#include "png_utils.h"
#include "types.h"
#include "wrapper_utils.h"
#include <node_api.h>
#include <stdlib.h>
#include <string.h>

static napi_value EncodeTextPng(napi_env env, napi_callback_info info) {
  Config config;
  config.mode = "encode";
  if (parse_napi_args(env, info, &config) != 0) {
    return NULL;
  }

  const char *mime_type = get_mime_type(config.input);
  if (strcmp(mime_type, "image/png") != 0) {
    napi_throw_error(env, NULL, "Input file must be a PNG image");
    return NULL;
  }

  int result = embed_message_in_png(config.input, config.output, config.message,
                                    config.header);
  if (result != 0) {
    napi_throw_error(env, NULL, "Failed to embed message");
    return NULL;
  }

  napi_value return_value;
  napi_status status = napi_get_boolean(env, true, &return_value);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to create return value");
    return NULL;
  }

  free(config.input);
  free(config.output);
  free(config.message);
  free(config.header);

  return return_value;
}

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_value fn;

  status = napi_create_function(env, "encodeTextPng", NAPI_AUTO_LENGTH,
                                EncodeTextPng, NULL, &fn);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to create function");
    return NULL;
  }

  status = napi_set_named_property(env, exports, "encodeTextPng", fn);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to set function");
    return NULL;
  }
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
