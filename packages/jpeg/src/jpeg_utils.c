#include "jpeg_utils.h"
#include <stdio.h>

#define TERMINATOR 0xFF

// Embed message in the DCT coefficients
void embed_message(JBLOCKARRAY row_ptrs, JDIMENSION width_in_blocks,
                   const char *message, size_t *bit_index,
                   size_t message_length) {
  for (JDIMENSION col = 0; col < width_in_blocks; col++) {
    JCOEF *block = row_ptrs[0][col];

    for (int i = 1; i <= DCTSIZE2; i++) {
      if (*bit_index < message_length) {
        int bit = (message[*bit_index / 8] >> (7 - (*bit_index % 8))) & 1;
        block[i] = (block[i] & ~1) | bit;

        (*bit_index)++;
      } else if (*bit_index < message_length + 8) {
        int bit = (TERMINATOR >> (7 - (*bit_index - message_length))) & 1;
        block[i] = (block[i] & ~1) | bit;

        (*bit_index)++;
      } else {
        return;
      }
    }
  }
}

// Extract message from the DCT coefficients
void extract_message(JBLOCKARRAY row_ptrs, JDIMENSION width_in_blocks,
                     char *buffer, size_t *bit_index, size_t buffer_size) {
  unsigned char current_byte = 0;
  size_t bits_collected = 0;
  size_t message_length = 0;
  int done = 0;

  for (JDIMENSION col = 0; col < width_in_blocks && !done; col++) {
    JCOEF *block = row_ptrs[0][col];

    for (int i = 1; i <= DCTSIZE2; i++) {
      int bit = (unsigned int)(block[i]) & 1;

      current_byte = (current_byte << 1) | bit;
      bits_collected++;

      if (bits_collected == 8) {

        if (current_byte == TERMINATOR) {
          printf("Terminator reached — stopping extraction\n");
          buffer[message_length] = '\0';
          done = 1;
          break;
        }

        if (message_length < buffer_size - 1) {
          buffer[message_length++] = current_byte;
        } else {
          printf("Buffer overflow prevented at index %zu\n", message_length);
          buffer[message_length] = '\0';
          done = 1;
          break;
        }

        current_byte = 0;
        bits_collected = 0;
      }
    }
  }

  buffer[message_length] = '\0';

  if (message_length > 100) {
    printf("No terminator found — stopping extraction\n");
  }
}