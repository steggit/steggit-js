#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif
#include "general_utils.h"
#include "jpeg_utils.h"
#include "png_utils.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>
#define DEFAULT_HEADER "$$"

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int encode_png(const char *input, const char *output, const char *message,
               const char *header, char **error_message) {

  if (!message || !strlen(message)) {
    *error_message = strdup("Error: Message is required");
    return 1;
  }
  if (!input || !strlen(input)) {
    *error_message = strdup("Error: Input file is required");
    return 1;
  }
  if (!output || !strlen(output)) {
    *error_message = strdup("Error: Output file is required");
    return 1;
  }
  if (is_valid_file(input) != 0) {
    *error_message = strdup("Error: Input file does not exist");
    return 1;
  }
  const char *mime_type = get_mime_type(input);
  if (strcmp(mime_type, "image/png") != 0) {
    *error_message = strdup("Error: Input file must be a PNG image");
    return 1;
  }
  if (!header) {
    header = DEFAULT_HEADER;
  }

  return embed_message_in_png(input, output, message, header, error_message);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int encode_jpeg(const char *input, const char *output, const char *message,
                const char *header, char **error_message) {

  if (!message || !strlen(message)) {
    *error_message = strdup("Error: Message is required");
    return 1;
  }
  if (!input || !strlen(input)) {
    *error_message = strdup("Error: Input file is required");
    return 1;
  }
  if (!output || !strlen(output)) {
    *error_message = strdup("Error: Output file is required");
    return 1;
  }
  if (is_valid_file(input) != 0) {
    *error_message = strdup("Error: Input file does not exist");
    return 1;
  }
  const char *mime_type = get_mime_type(input);
  if (strcmp(mime_type, "image/jpeg") != 0) {
    *error_message = strdup("Error: Input file must be a JPEG image");
    return 1;
  }
  if (!header) {
    header = DEFAULT_HEADER;
  }

  return embed_message_in_jpeg(input, output, message, header, error_message);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
char *decode_png(const char *input, const char *header, char **error_message) {

  if (is_valid_file(input) != 0) {
    *error_message = strdup("Error: Input file does not exist");
    return NULL;
  }
  const char *mime_type = get_mime_type(input);
  if (strcmp(mime_type, "image/png") != 0) {
    *error_message = strdup("Error: Input file must be a PNG image");
    return NULL;
  }
  if (!header) {
    header = DEFAULT_HEADER;
  }
  char *message = extract_message_from_png(input, header, error_message);
  if (message == NULL) {
    *error_message = strdup("Error: Failed to extract message from PNG image");
    return NULL;
  }
  return message;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
char *decode_jpeg(const char *input, const char *header, char **error_message) {

  if (is_valid_file(input) != 0) {
    *error_message = strdup("Error: Input file does not exist");
    return NULL;
  }
  const char *mime_type = get_mime_type(input);
  if (strcmp(mime_type, "image/jpeg") != 0) {
    *error_message = strdup("Error: Input file must be a JPEG image");
    return NULL;
  }
  if (!header) {
    header = DEFAULT_HEADER;
  }
  char *message = extract_message_from_jpeg(input, header, error_message);
  if (message == NULL) {
    *error_message = strdup("Error: Failed to extract message from JPEG image");
    return NULL;
  }
  return message;
}

#ifndef __EMSCRIPTEN__
int main(int argc, char *argv[]) {
  Config config = {0};
  int result = parse_args(argc, argv, &config);
  if (result != 0) {
    exit(result);
  }

  const char *mime_type = get_mime_type(config.input);
  if (strcmp(mime_type, "image/jpeg") != 0 &&
      strcmp(mime_type, "image/png") != 0) {
    fprintf(stderr, "Error: Input file must be a JPEG or PNG image\n");
    exit(1);
  }

  if (strcmp(config.mode, "decode") == 0) {
    char *message = NULL;
    char *error_message = NULL;
    if (strcmp(mime_type, "image/jpeg") == 0) {
      message = decode_jpeg(config.input, config.header, &error_message);
    } else {
      message = decode_png(config.input, config.header, &error_message);
    }

    if (message == NULL) {
      fprintf(stderr, "%s\n", error_message);
      free(error_message);
      exit(1);
    }

    printf("%s\n", message);
    free(message);
    free(error_message);
    exit(0);
  }

  if (strcmp(config.mode, "encode") == 0) {
    int result = -1;
    char *error_message = NULL;
    if (strcmp(mime_type, "image/jpeg") == 0) {
      result = encode_jpeg(config.input, config.output, config.message,
                           config.header, &error_message);
    } else {
      result = encode_png(config.input, config.output, config.message,
                          config.header, &error_message);
    }

    if (result != 0) {
      fprintf(stderr, "%s\n", error_message);
      free(error_message);
      exit(1);
    }

    free(error_message);
    exit(0);
  }

  printf("Unknown command: %s\n", argv[1]);
  printf("Usage:\n");
  printf("  Encode: %s encode --input <input> --output <output> --message "
         "<message> --header <header>\n",
         argv[0]);
  printf("  Decode: %s decode --input <input> --header <header>\n", argv[0]);
  exit(1);
}
#endif // __EMSCRIPTEN__