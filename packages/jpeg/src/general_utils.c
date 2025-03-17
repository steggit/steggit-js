#include "general_utils.h"
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define JPEG_MAGIC "\xFF\xD8\xFF"
#define PNG_MAGIC "\x89PNG\r\n\x1a\n"
#define JPEG_MAGIC_SIZE 3
#define PNG_MAGIC_SIZE 8

const char *get_mime_type(const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
    return NULL;
  }

  uint8_t buffer[8];
  size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);
  fclose(file);

  if (bytesRead >= JPEG_MAGIC_SIZE &&
      memcmp(buffer, JPEG_MAGIC, JPEG_MAGIC_SIZE) == 0) {
    return "image/jpeg";
  }

  if (bytesRead >= PNG_MAGIC_SIZE &&
      memcmp(buffer, PNG_MAGIC, PNG_MAGIC_SIZE) == 0) {
    return "image/png";
  }

  return NULL; // Unsupported file type
}

int parse_args(int argc, char *argv[], struct Config *config) {
  int opt;

  static struct option long_options[] = {{"input", required_argument, 0, 'i'},
                                         {"output", required_argument, 0, 'o'},
                                         {"message", required_argument, 0, 'm'},
                                         {"header", required_argument, 0, 'h'},
                                         {0, 0, 0, 0}};

  const char *encode_usage = "Usage: %s encode --input <input> --output "
                             "<output> --message <message> --header <header>\n";
  const char *decode_usage =
      "Usage: %s decode --input <input> --header <header>\n";
  const char *general_usage =
      "Usage:\n  Encode: %s encode --input <input> --output <output> --message "
      "<message> --header <header>\n  Decode: %s decode --input <input> "
      "--header <header>\n";

  if (argc < 2) {
    printf(general_usage, argv[0], argv[0]);
    return 1;
  }

  config->mode = argv[1];

  // Skip the command argument for getopt processing
  optind = 2;

  if (strcmp(config->mode, "encode") == 0) {
    while ((opt = getopt_long(argc, argv, "i:o:m:h:", long_options, NULL)) !=
           -1) {
      switch (opt) {
      case 'i':
        config->input = optarg;
        break;
      case 'o':
        config->output = optarg;
        break;
      case 'm':
        config->message = optarg;
        break;
      case 'h':
        config->header = optarg;
        break;
      default:
        printf(encode_usage, argv[0]);
        return 1;
      }
    }

    if (!config->input || !config->output || !config->message) {
      printf(encode_usage, argv[0]);
      return 1;
    }

    return 0;
  }

  if (strcmp(config->mode, "decode") == 0) {
    while ((opt = getopt_long(argc, argv, "i:h:", long_options, NULL)) != -1) {
      switch (opt) {
      case 'i':
        config->input = optarg;
        break;
      case 'h':
        config->header = optarg;
        break;
      default:
        printf(decode_usage, argv[0]);
        return 1;
      }
    }

    if (!config->input) {
      printf(decode_usage, argv[0]);
      return 1;
    }

    return 0;
  }

  // If we get here, mode is invalid
  printf(general_usage, argv[0], argv[0]);
  return 1;
}