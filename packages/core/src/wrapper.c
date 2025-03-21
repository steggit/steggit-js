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

static void ExecuteWork(napi_env env, void *data) {
  PromiseData *promise_data = (PromiseData *)data;
  Config *config = &promise_data->config;

  FILE *file = fopen(config->input, "rb");
  if (file == NULL) {
    promise_data->error = strdup("Input file does not exist");
    promise_data->result = -1;
    return;
  }
  fclose(file);

  const char *mime_type = get_mime_type(config->input);
  if (strcmp(config->file_ext, "png") == 0 &&
      strcmp(mime_type, "image/png") != 0) {
    promise_data->error = strdup("Input file must be a PNG image");
    promise_data->result = -1;
    return;
  }

  if (strcmp(config->file_ext, "jpeg") == 0 &&
      strcmp(mime_type, "image/jpeg") != 0) {
    promise_data->error = strdup("Input file must be a JPEG image");
    promise_data->result = -1;
    return;
  }

  if (strcmp(config->mode, "encode") == 0 &&
      strcmp(config->file_ext, "png") == 0) {
    promise_data->result =
        embed_message_in_png(config->input, config->output, config->message,
                             config->header, &promise_data->error);
  } else if (strcmp(config->mode, "encode") == 0 &&
             strcmp(config->file_ext, "jpeg") == 0) {
    promise_data->result =
        embed_message_in_jpeg(config->input, config->output, config->message,
                              config->header, &promise_data->error);
  } else if (strcmp(config->mode, "decode") == 0 &&
             strcmp(config->file_ext, "png") == 0) {
    promise_data->output = extract_message_from_png(
        config->input, config->header, &promise_data->error);
  } else if (strcmp(config->mode, "decode") == 0 &&
             strcmp(config->file_ext, "jpeg") == 0) {
    promise_data->output = extract_message_from_jpeg(
        config->input, config->header, &promise_data->error);
  }

  bool embed_failed =
      strcmp(config->mode, "encode") == 0 && promise_data->result != 0;
  bool decode_failed =
      strcmp(config->mode, "decode") == 0 && promise_data->output == NULL;

  if (decode_failed) {
    promise_data->error = strdup("Failed to decode message");
    promise_data->result = -1;
  } else if (embed_failed) {
    promise_data->result = -1;
  } else {
    promise_data->result = 0;
  }
}

static void CompleteWork(napi_env env, napi_status status, void *data) {
  PromiseData *promise_data = (PromiseData *)data;

  if (status == napi_ok && promise_data->result == 0) {
    napi_value result;
    if (promise_data->output != NULL) {
      napi_create_string_utf8(env, promise_data->output,
                              strlen(promise_data->output), &result);
    } else {
      napi_get_undefined(env, &result);
    }
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

static napi_value HandleAsyncWork(napi_env env, PromiseData *promise_data) {
  napi_value promise;
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

  if (strcmp(promise_data->config.mode, "encode") == 0 &&
      (promise_data->config.output == NULL ||
       strlen(promise_data->config.output) == 0)) {
    handle_encode_error(env, promise_data, "Output file is required");
    return promise;
  }

  if (strcmp(promise_data->config.mode, "encode") == 0 &&
      (promise_data->config.message == NULL ||
       strlen(promise_data->config.message) == 0)) {
    handle_encode_error(env, promise_data, "Message is required");
    return promise;
  }

  // Create a string to identify the async work
  napi_value work_name;
  status = napi_create_string_utf8(env, "AsyncWork", strlen("AsyncWork"),
                                   &work_name);
  if (status != napi_ok) {
    handle_encode_error(env, promise_data, "Unable to create work name");
    return promise;
  }

  // Create async work
  status =
      napi_create_async_work(env, NULL, work_name, ExecuteWork, CompleteWork,
                             promise_data, &promise_data->work);
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

static napi_value EncodeTextPng(napi_env env, napi_callback_info info) {
  PromiseData *promise_data = (PromiseData *)malloc(sizeof(PromiseData));
  if (promise_data == NULL) {
    napi_throw_error(env, NULL, "Failed to allocate memory");
    return NULL;
  }
  memset(&promise_data->config, 0, sizeof(Config));
  promise_data->config.mode = strdup("encode");
  promise_data->config.file_ext = strdup("png");
  promise_data->result = 0;

  if (parse_napi_args(env, info, &promise_data->config) != 0) {
    free_config(&promise_data->config);
    free(promise_data);
    return NULL;
  }
  return HandleAsyncWork(env, promise_data);
}

static napi_value EncodeTextJpeg(napi_env env, napi_callback_info info) {
  PromiseData *promise_data = (PromiseData *)malloc(sizeof(PromiseData));
  if (promise_data == NULL) {
    napi_throw_error(env, NULL, "Failed to allocate memory");
    return NULL;
  }
  memset(&promise_data->config, 0, sizeof(Config));
  promise_data->config.mode = strdup("encode");
  promise_data->config.file_ext = strdup("jpeg");
  promise_data->result = 0;

  if (parse_napi_args(env, info, &promise_data->config) != 0) {
    free_config(&promise_data->config);
    free(promise_data);
    return NULL;
  }
  return HandleAsyncWork(env, promise_data);
}

static napi_value DecodeTextPng(napi_env env, napi_callback_info info) {
  PromiseData *promise_data = (PromiseData *)malloc(sizeof(PromiseData));
  if (promise_data == NULL) {
    napi_throw_error(env, NULL, "Failed to allocate memory");
    return NULL;
  }
  memset(&promise_data->config, 0, sizeof(Config));
  promise_data->config.mode = strdup("decode");
  promise_data->config.file_ext = strdup("png");
  promise_data->result = 0;

  if (parse_napi_args(env, info, &promise_data->config) != 0) {
    free_config(&promise_data->config);
    free(promise_data);
    return NULL;
  }
  return HandleAsyncWork(env, promise_data);
}

static napi_value DecodeTextJpeg(napi_env env, napi_callback_info info) {
  PromiseData *promise_data = (PromiseData *)malloc(sizeof(PromiseData));
  if (promise_data == NULL) {
    napi_throw_error(env, NULL, "Failed to allocate memory");
    return NULL;
  }
  memset(&promise_data->config, 0, sizeof(Config));
  promise_data->config.mode = strdup("decode");
  promise_data->config.file_ext = strdup("jpeg");
  promise_data->result = 0;

  if (parse_napi_args(env, info, &promise_data->config) != 0) {
    free_config(&promise_data->config);
    free(promise_data);
    return NULL;
  }
  return HandleAsyncWork(env, promise_data);
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
    status =
        napi_create_function(env, functions[i].name, strlen(functions[i].name),
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
