#include "general_utils.h"
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