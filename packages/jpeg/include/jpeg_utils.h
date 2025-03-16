#ifndef JPEG_UTILS_H
#define JPEG_UTILS_H

#include <_stdio.h>
#include <jpeglib.h>

void embed_message(JBLOCKARRAY row_ptrs, JDIMENSION width_in_blocks,
                   const char *message, size_t *bit_index,
                   size_t message_length);

char *extract_message(JBLOCKARRAY row_ptrs, JDIMENSION width_in_blocks,
                      JDIMENSION height_in_blocks, const char *header);

int embed_message_in_jpeg(const char *input_path, const char *output_path,
                          const char *message, const char *header);

int extract_message_from_jpeg(const char *input_path, const char *header);

#endif // JPEG_UTILS_H