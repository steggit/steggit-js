#include "file_utils.h"
#include "jpeg_utils.h"
#include "png_utils.h"
#include <stdio.h>
#include <string.h>
#define DEFAULT_HEADER "$$"

int main(int argc, char *argv[]) {

  if (argc < 3) {
    printf("Usage:\n");
    printf("  Encode: %s encode <input> <output> <message> [header]\n",
           argv[0]);
    printf("  Decode: %s decode <input> [header]\n", argv[0]);
    return 1;
  }

  const char *mime_type = get_mime_type(argv[2]);
  if (strcmp(mime_type, "image/jpeg") != 0 &&
      strcmp(mime_type, "image/png") != 0) {
    fprintf(stderr, "Error: Input file must be a JPEG or PNG image\n");
    return 1;
  }

  if (strcmp(argv[1], "decode") == 0) {
    // Decode Mode
    if (argc < 3 || argc > 4) {
      printf("Decode usage: %s decode <input> [header]\n", argv[0]);
      return 1;
    }
    const char *header = (argc == 4) ? argv[3] : DEFAULT_HEADER;

    if (strcmp(mime_type, "image/jpeg") == 0) {
      if (extract_message_from_jpeg(argv[2], header) != 0) {
        fprintf(stderr, "Failed to extract message\n");
        return 1;
      }
    } else if (strcmp(mime_type, "image/png") == 0) {
      if (extract_message_from_png(argv[2], header) != 0) {
        fprintf(stderr, "Failed to extract message\n");
        return 1;
      }
    }
  } else if (strcmp(argv[1], "encode") == 0) {
    // Encode Mode
    if (argc < 5 || argc > 6) {
      printf("Encode usage: %s encode <input> <output> <message> [header]\n",
             argv[0]);
      return 1;
    }
    const char *header = (argc == 6) ? argv[5] : DEFAULT_HEADER;
    int result = -1;

    if (strcmp(mime_type, "image/jpeg") == 0) {
      result = embed_message_in_jpeg(argv[2], argv[3], argv[4], header);
    } else if (strcmp(mime_type, "image/png") == 0) {
      result = embed_message_in_png(argv[2], argv[3], argv[4], header);
    }

    if (result != 0) {
      fprintf(stderr, "Failed to embed message\n");
      return 1;
    }
  } else {
    printf("Unknown command: %s\n", argv[1]);
    printf("Usage:\n");
    printf("  Encode: %s encode <input> <output> <message> [header]\n",
           argv[0]);
    printf("  Decode: %s decode <input> [header]\n", argv[0]);
    return 1;
  }

  return 0;
}