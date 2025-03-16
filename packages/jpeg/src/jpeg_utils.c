#include "jpeg_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TERMINATOR 0xFF
#define MAX_BUFFER_SIZE 65536 // 64KB
#define MIN(a, b) ((a) < (b) ? (a) : (b))


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
char* extract_message(JBLOCKARRAY row_ptrs, JDIMENSION width_in_blocks, JDIMENSION height_in_blocks, const char *header) {
  unsigned char current_byte = 0;
  size_t bits_collected = 0;
  size_t message_length = 0;
  int done = 0;
  int header_found = 0;
  size_t header_length = strlen(header);

  size_t available_bits = (width_in_blocks * height_in_blocks) * 63;
  size_t max_message_length = available_bits / 8;

  char *buffer = (char *)malloc(MIN(max_message_length, MAX_BUFFER_SIZE));
  if (buffer == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return NULL;
  }

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

        if (message_length < max_message_length - 1) {
          buffer[message_length++] = current_byte;
          
          // Check if we've found the header
          if (!header_found && header_length > 0 && message_length >= header_length) {
            if (strncmp(buffer + message_length - header_length, header, header_length) == 0) {
              header_found = 1;
              // Reset message_length to start after the header
              message_length = 0;
            }
          }
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

  if (!header_found && header_length > 0) {
    // Header not found, return empty string
    buffer[0] = '\0';
  }

  return buffer;
}