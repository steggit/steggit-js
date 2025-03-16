#ifndef JPEG_UTILS_H
#define JPEG_UTILS_H

#include <_stdio.h>
#include <jpeglib.h>

void embed_message(JBLOCKARRAY row_ptrs, JDIMENSION width_in_blocks,
                   const char *message, size_t *bit_index,
                   size_t message_length);

char *extract_message(JBLOCKARRAY row_ptrs, JDIMENSION width_in_blocks,
                      JDIMENSION height_in_blocks);

#endif // JPEG_UTILS_H