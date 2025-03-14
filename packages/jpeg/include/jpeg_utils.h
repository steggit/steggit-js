#ifndef JPEG_UTILS_H
#define JPEG_UTILS_H

#include <_stdio.h>
#include <jpeglib.h>

void embed_message(JBLOCKARRAY row_ptrs, JDIMENSION width_in_blocks,
                   const char *message, size_t *bit_index, size_t message_length);

void extract_message(JBLOCKARRAY row_ptrs, JDIMENSION width_in_blocks,
                     char *buffer, size_t *bit_index, size_t buffer_size);

#endif // JPEG_UTILS_H