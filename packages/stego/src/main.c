#include "general_utils.h"
#include "jpeg_utils.h"
#include "png_utils.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>
#define DEFAULT_HEADER "$$"

int main(int argc, char *argv[]) {
  Config config = {0};
  int result = parse_args(argc, argv, &config);
  if (result != 0) {
    exit(result);
  }

  if (!config.header) {
    config.header = DEFAULT_HEADER;
  }

  const char *mime_type = get_mime_type(config.input);
  if (strcmp(mime_type, "image/jpeg") != 0 &&
      strcmp(mime_type, "image/png") != 0) {
    fprintf(stderr, "Error: Input file must be a JPEG or PNG image\n");
    exit(1);
  }

  if (strcmp(config.mode, "decode") == 0) {
    char *message = NULL;

    if (strcmp(mime_type, "image/jpeg") == 0) {
      message = extract_message_from_jpeg(config.input, config.header);
    } else {
      message = extract_message_from_png(config.input, config.header);
    }

    if (message == NULL) {
      fprintf(stderr, "Failed to extract message\n");
      exit(1);
    }

    printf("%s\n", message);
    free(message);
    exit(0);
  }

  if (strcmp(config.mode, "encode") == 0) {
    int result = -1;

    if (strcmp(mime_type, "image/jpeg") == 0) {
      result = embed_message_in_jpeg(config.input, config.output,
                                     config.message, config.header);
    } else {
      result = embed_message_in_png(config.input, config.output, config.message,
                                    config.header);
    }

    if (result != 0) {
      fprintf(stderr, "Failed to embed message\n");
      exit(1);
    }

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