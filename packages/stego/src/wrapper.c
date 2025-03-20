#define NAPI_VERSION 9
#include "general_utils.h"
#include "jpeg_utils.h"
#include "png_utils.h"
#include "types.h"
#include "wrapper_utils.h"
#include <node_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void ExecutePngEncodeWork(napi_env env, void *data) {
  EncodePromiseData *promise_data = (EncodePromiseData *)data;
  Config *config = &promise_data->config;

  FILE *file = fopen(config->input, "rb");
  if (file == NULL) {
    promise_data->error = strdup("Input file does not exist");
    promise_data->result = -1;
    return;
  }
  fclose(file);

  const char *mime_type = get_mime_type(config->input);
  if (strcmp(mime_type, "image/png") != 0) {
    promise_data->error = strdup("Input file must be a PNG image");
    promise_data->result = -1;
    return;
  }

  promise_data->result = embed_message_in_png(config->input, config->output,
                                              config->message, config->header);
  if (promise_data->result != 0) {
    promise_data->error = strdup("Failed to embed message");
    promise_data->result = -1;
  }
}

static void CompleteEncodeWork(napi_env env, napi_status status, void *data) {
  EncodePromiseData *promise_data = (EncodePromiseData *)data;

  if (status == napi_ok && promise_data->result == 0) {
    napi_value result;
    napi_get_undefined(env, &result);
    napi_resolve_deferred(env, promise_data->deferred, result);
    free_config(&promise_data->config);
    napi_delete_async_work(env, promise_data->work);
    free(promise_data);
    return;
  }
  if (promise_data->error == NULL) {
    handle_encode_error(env, promise_data, "Unknown error occurred");
  } else {
    handle_encode_error(env, promise_data, promise_data->error);
  }
}

static napi_value EncodeTextPng(napi_env env, napi_callback_info info) {
  napi_value promise;
  EncodePromiseData *promise_data =
      (EncodePromiseData *)malloc(sizeof(EncodePromiseData));
  if (promise_data == NULL) {
    napi_throw_error(env, NULL, "Failed to allocate memory");
    return NULL;
  }
  memset(&promise_data->config, 0, sizeof(Config));
  promise_data->config.mode = strdup("encode");
  promise_data->result = 0;

  if (parse_napi_args(env, info, &promise_data->config) != 0) {
    free_config(&promise_data->config);
    free(promise_data);
    return NULL;
  }
  napi_status status =
      napi_create_promise(env, &promise_data->deferred, &promise);
  if (status != napi_ok) {
    free_config(&promise_data->config);
    free(promise_data);
    napi_throw_error(env, NULL, "Unable to create promise");
    return NULL;
  }

  // Validate required parameters
  if (promise_data->config.input == NULL ||
      strlen(promise_data->config.input) == 0) {
    handle_encode_error(env, promise_data, "Input file is required");
    return promise;
  }

  if (promise_data->config.output == NULL ||
      strlen(promise_data->config.output) == 0) {
    handle_encode_error(env, promise_data, "Output file is required");
    return promise;
  }

  if (promise_data->config.message == NULL ||
      strlen(promise_data->config.message) == 0) {
    handle_encode_error(env, promise_data, "Message is required");
    return promise;
  }

  // Create a string to identify the async work
  napi_value work_name;
  status = napi_create_string_utf8(env, "EncodeTextPngWork", NAPI_AUTO_LENGTH,
                                   &work_name);
  if (status != napi_ok) {
    handle_encode_error(env, promise_data, "Unable to create work name");
    return promise;
  }

  // Create async work
  status = napi_create_async_work(env, NULL, work_name, ExecutePngEncodeWork,
                                  CompleteEncodeWork, promise_data,
                                  &promise_data->work);
  if (status != napi_ok) {
    handle_encode_error(env, promise_data, "Unable to create async work");
    return promise;
  }

  // Queue the work
  status = napi_queue_async_work(env, promise_data->work);
  if (status != napi_ok) {
    handle_encode_error(env, promise_data, "Unable to queue async work");
    return promise;
  }

  return promise;
}

static napi_value EncodeTextJpeg(napi_env env, napi_callback_info info) {
  Config config;
  config.mode = "encode";
  if (parse_napi_args(env, info, &config) != 0) {
    return NULL;
  }

  if (config.input == NULL || strlen(config.input) == 0) {
    napi_throw_error(env, NULL, "Input file is required");
    return NULL;
  }

  if (config.output == NULL || strlen(config.output) == 0) {
    napi_throw_error(env, NULL, "Output file is required");
    return NULL;
  }

  if (config.message == NULL || strlen(config.message) == 0) {
    napi_throw_error(env, NULL, "Message is required");
    return NULL;
  }

  FILE *file = fopen(config.input, "rb");
  if (file == NULL) {
    napi_throw_error(env, NULL, "Input file does not exist");
    return NULL;
  }
  fclose(file);

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

  if (config.input == NULL || strlen(config.input) == 0) {
    napi_throw_error(env, NULL, "Input file is required");
    return NULL;
  }

  FILE *file = fopen(config.input, "rb");
  if (file == NULL) {
    napi_throw_error(env, NULL, "Input file does not exist");
    return NULL;
  }
  fclose(file);

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

  if (config.input == NULL || strlen(config.input) == 0) {
    napi_throw_error(env, NULL, "Input file is required");
    return NULL;
  }

  FILE *file = fopen(config.input, "rb");
  if (file == NULL) {
    napi_throw_error(env, NULL, "Input file does not exist");
    return NULL;
  }
  fclose(file);

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
