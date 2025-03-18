#include "wrapper_utils.h"
#include <stdlib.h>

int parse_napi_args(napi_env env, napi_callback_info info, Config *config) {
  napi_value args[4];
  size_t argc = 4;
  napi_status status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to get arguments");
    return 1;
  }

  napi_value input_file_value = args[0];
  napi_value output_file_value = args[1];
  napi_value message_value = args[2];
  napi_value header_value = args[3];

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