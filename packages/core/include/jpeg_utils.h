#ifndef JPEG_UTILS_H
#define JPEG_UTILS_H

#ifdef __EMSCRIPTEN__
#include <stddef.h> // For size_t - not included in emscripten jpeg port
#include <stdio.h>  // For FILE - not included in emscripten jpeg port
#else
#include <stdio.h>
#endif

#include <jpeglib.h>

void embed_message(JBLOCKARRAY row_ptrs, JDIMENSION width_in_blocks,
                   const char *message, size_t *bit_index,
                   size_t message_length);

char *extract_message(JBLOCKARRAY row_ptrs, JDIMENSION width_in_blocks,
                      JDIMENSION height_in_blocks, const char *header,
                      char **error_message);

int embed_message_in_jpeg(const char *input_path, const char *output_path,
                          const char *message, const char *header,
                          char **error_message);

char *extract_message_from_jpeg(const char *input_path, const char *header,
                                char **error_message);

#endif // JPEG_UTILS_H