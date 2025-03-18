#define NAPI_VERSION 9
#include "general_utils.h"
#include "jpeg_utils.h"
#include "png_utils.h"
#include "types.h"
#include "wrapper_utils.h"
#include <node_api.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_HEADER "$$"

void free_config(Config *config) {
  if (config == NULL) {
    return;
  }
  if (config->mode != NULL) {
    free(config->mode);
  }
  if (config->input != NULL) {
    free(config->input);
  }
  if (config->output != NULL) {
    free(config->output);
  }
  if (config->message != NULL) {
    free(config->message);
  }
  if (config->header != NULL) {
    free(config->header);
  }
}

static napi_value EncodeTextPng(napi_env env, napi_callback_info info) {
  Config config;
  config.mode = "encode";
  if (parse_napi_args(env, info, &config) != 0) {
    return NULL;
  }

  if (config.input == NULL) {
    napi_throw_error(env, NULL, "Input file is required");
    return NULL;
  }

  if (config.output == NULL) {
    napi_throw_error(env, NULL, "Output file is required");
    return NULL;
  }

  if (config.message == NULL) {
    napi_throw_error(env, NULL, "Message is required");
    return NULL;
  }

  if (config.header == NULL) {
    config.header = DEFAULT_HEADER;
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

  free_config(&config);

  return return_value;
}

static napi_value EncodeTextJpeg(napi_env env, napi_callback_info info) {
  Config config;
  config.mode = "encode";
  if (parse_napi_args(env, info, &config) != 0) {
    return NULL;
  }

  if (config.input == NULL) {
    napi_throw_error(env, NULL, "Input file is required");
    return NULL;
  }

  if (config.output == NULL) {
    napi_throw_error(env, NULL, "Output file is required");
    return NULL;
  }

  if (config.message == NULL) {
    napi_throw_error(env, NULL, "Message is required");
    return NULL;
  }

  if (config.header == NULL) {
    config.header = DEFAULT_HEADER;
  }

  const char *mime_type = get_mime_type(config.input);
  if (strcmp(mime_type, "image/jpeg") != 0) {
    napi_throw_error(env, NULL, "Input file must be a JPEG image");
    return NULL;
  }

  int result = embed_message_in_jpeg(config.input, config.output,
                                     config.message, config.header);
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

  free_config(&config);

  return return_value;
}

static napi_value DecodeTextPng(napi_env env, napi_callback_info info) {
  Config config;
  config.mode = "decode";
  if (parse_napi_args(env, info, &config) != 0) {
    return NULL;
  }

  if (config.input == NULL) {
    napi_throw_error(env, NULL, "Input file is required");
    return NULL;
  }

  if (config.header == NULL) {
    config.header = DEFAULT_HEADER;
  }

  const char *mime_type = get_mime_type(config.input);
  if (strcmp(mime_type, "image/png") != 0) {
    napi_throw_error(env, NULL, "Input file must be a PNG image");
    return NULL;
  }

  char *message = extract_message_from_png(config.input, config.header);
  if (message == NULL) {
    napi_throw_error(env, NULL, "Failed to extract message");
    return NULL;
  }

  napi_value return_value;
  napi_status status =
      napi_create_string_utf8(env, message, strlen(message), &return_value);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to create return value");
    return NULL;
  }

  free(message);
  free_config(&config);

  return return_value;
}

static napi_value DecodeTextJpeg(napi_env env, napi_callback_info info) {
  Config config;
  config.mode = "decode";
  if (parse_napi_args(env, info, &config) != 0) {
    return NULL;
  }

  if (config.input == NULL) {
    napi_throw_error(env, NULL, "Input file is required");
    return NULL;
  }

  if (config.header == NULL) {
    config.header = DEFAULT_HEADER;
  }

  const char *mime_type = get_mime_type(config.input);
  if (strcmp(mime_type, "image/jpeg") != 0) {
    napi_throw_error(env, NULL, "Input file must be a JPEG image");
    return NULL;
  }

  char *message = extract_message_from_jpeg(config.input, config.header);
  if (message == NULL) {
    napi_throw_error(env, NULL, "Failed to extract message");
    return NULL;
  }

  napi_value return_value;
  napi_status status =
      napi_create_string_utf8(env, message, strlen(message), &return_value);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to create return value");
    return NULL;
  }

  free(message);
  free_config(&config);

  return return_value;
}

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_value fn;

  // Function definitions with their handlers
  struct {
    const char *name;
    napi_callback handler;
  } functions[] = {{"encodeTextPng", EncodeTextPng},
                   {"encodeTextJpeg", EncodeTextJpeg},
                   {"decodeTextPng", DecodeTextPng},
                   {"decodeTextJpeg", DecodeTextJpeg}};

  int num_functions = sizeof(functions) / sizeof(functions[0]);
  for (int i = 0; i < num_functions; i++) {
    status = napi_create_function(env, functions[i].name, NAPI_AUTO_LENGTH,
                                  functions[i].handler, NULL, &fn);
    if (status != napi_ok) {
      char error_message[100];
      snprintf(error_message, sizeof(error_message),
               "Failed to create function %s", functions[i].name);
      napi_throw_error(env, NULL, error_message);
      return NULL;
    }

    status = napi_set_named_property(env, exports, functions[i].name, fn);
    if (status != napi_ok) {
      char error_message[100];
      snprintf(error_message, sizeof(error_message),
               "Failed to set function %s", functions[i].name);
      napi_throw_error(env, NULL, error_message);
      return NULL;
    }
  }

  return exports;
}
NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
