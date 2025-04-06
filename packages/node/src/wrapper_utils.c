#include "wrapper_utils.h"
#include <stdlib.h>
#include <string.h>
#define DEFAULT_HEADER "$$"

int parse_napi_args(napi_env env, napi_callback_info info, Config *config) {
  napi_value args[4];
  size_t argc = 4;
  napi_status status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to get arguments");
    return 1;
  }

  napi_value input_file_value = NULL;
  napi_value output_file_value = NULL;
  napi_value message_value = NULL;
  napi_value header_value = NULL;

  if (config->mode == NULL) {
    napi_throw_error(env, NULL, "Mode is required");
    return 1;
  }

  if (strcmp(config->mode, "encode") == 0 && argc == 4) {
    input_file_value = args[0];
    output_file_value = args[1];
    message_value = args[2];
    header_value = args[3];
  } else if (strcmp(config->mode, "encode") == 0 && argc == 3) {
    input_file_value = args[0];
    output_file_value = args[1];
    message_value = args[2];
  } else if (strcmp(config->mode, "decode") == 0 && argc == 2) {
    input_file_value = args[0];
    header_value = args[1];
  } else if (strcmp(config->mode, "decode") == 0 && argc == 1) {
    input_file_value = args[0];
  } else {
    napi_throw_error(env, NULL, "Invalid number of arguments");
    return 1;
  }

  if (header_value == NULL) {
    napi_create_string_utf8(env, DEFAULT_HEADER, strlen(DEFAULT_HEADER),
                            &header_value);
  }

  size_t input_length, output_length, message_length, header_length;
  napi_get_value_string_utf8(env, input_file_value, NULL, 0, &input_length);
  napi_get_value_string_utf8(env, output_file_value, NULL, 0, &output_length);
  napi_get_value_string_utf8(env, message_value, NULL, 0, &message_length);
  napi_get_value_string_utf8(env, header_value, NULL, 0, &header_length);

  config->input = (char *)malloc(input_length + 1);
  config->output = (char *)malloc(output_length + 1);
  config->message = (char *)malloc(message_length + 1);
  config->header = (char *)malloc(header_length + 1);

  if (config->input == NULL || config->output == NULL ||
      config->message == NULL || config->header == NULL) {
    napi_throw_error(env, NULL, "Failed to allocate memory");
    return 1;
  }

  napi_get_value_string_utf8(env, input_file_value, config->input,
                             input_length + 1, NULL);
  napi_get_value_string_utf8(env, output_file_value, config->output,
                             output_length + 1, NULL);
  napi_get_value_string_utf8(env, message_value, config->message,
                             message_length + 1, NULL);
  napi_get_value_string_utf8(env, header_value, config->header,
                             header_length + 1, NULL);

  return 0;
}

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
  if (config->file_ext != NULL) {
    free(config->file_ext);
  }
}

void handle_encode_error(napi_env env, PromiseData *promise_data,
                         const char *error_message) {
  napi_value msg, err;
  napi_create_string_utf8(env, error_message, strlen(error_message), &msg);
  napi_create_error(env, NULL, msg, &err);
  napi_reject_deferred(env, promise_data->deferred, err);
  if (promise_data->work != NULL) {
    napi_delete_async_work(env, promise_data->work);
  }
  free_config(&promise_data->config);
  free(promise_data);
}
