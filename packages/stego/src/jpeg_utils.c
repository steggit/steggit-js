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
      } else {
        return;
      }
    }
  }
}

// Extract message from the DCT coefficients
char *extract_message(JBLOCKARRAY row_ptrs, JDIMENSION width_in_blocks,
                      JDIMENSION height_in_blocks, const char *header,
                      char **error_message) {
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
    *error_message = strdup("Memory allocation failed");
    return NULL;
  }

  for (JDIMENSION col = 0; col < width_in_blocks && !done; col++) {
    JCOEF *block = row_ptrs[0][col];

    for (int i = 1; i <= DCTSIZE2; i++) {
      int bit = (unsigned int)(block[i]) & 1;

      current_byte = (current_byte << 1) | bit;
      bits_collected++;

      if (bits_collected == 8) {

        if (current_byte == TERMINATOR && header_found) {
          printf("Terminator reached — stopping extraction\n");
          buffer[message_length] = '\0';
          done = 1;
          break;
        }

        if (message_length < max_message_length - 1) {
          buffer[message_length++] = current_byte;
          // Check if we've found the header
          if (!header_found && header_length > 0 &&
              message_length >= header_length) {
            if (strncmp(buffer + message_length - header_length, header,
                        header_length) == 0) {
              header_found = 1;
              printf("Header found — resetting message_length\n");
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
    free(buffer);
    return NULL;
  }

  return buffer;
}

int embed_message_in_jpeg(const char *input_path, const char *output_path,
                          const char *message, const char *header,
                          char **error_message) {
  struct jpeg_decompress_struct cinfo;
  struct jpeg_compress_struct dstinfo;
  struct jpeg_error_mgr jerr;
  FILE *infile = NULL, *outfile = NULL;
  jvirt_barray_ptr *coef_arrays;

  infile = fopen(input_path, "rb");
  if (!infile) {
    *error_message = strdup("Failed to open input file");
    return -1;
  }

  char combined_message[strlen(header) + strlen(message) + 2];
  strcpy(combined_message, header);
  strcat(combined_message, message);
  char terminator = TERMINATOR;
  strncat(combined_message, &terminator, 1);

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  coef_arrays = jpeg_read_coefficients(&cinfo);

  size_t bit_index = 0;
  size_t combined_length = strlen(combined_message);
  size_t combined_bits = combined_length * 8;

  jpeg_component_info *comp = &cinfo.comp_info[0];
  JDIMENSION width_in_blocks = comp->width_in_blocks;
  JDIMENSION height_in_blocks = comp->height_in_blocks;

  printf("width_in_blocks: %d\n", width_in_blocks);
  printf("height_in_blocks: %d\n", height_in_blocks);
  printf("header: %s\n", header);
  printf("embed_message: %s\n", message);

  size_t available_bits = (width_in_blocks * height_in_blocks) * 63;
  size_t available_bytes = available_bits / 8;

  printf("available_bytes: %zu\n", available_bytes);
  printf("combined_length: %zu bytes (%zu bits)\n", combined_length,
         combined_bits);

  if (combined_bits > available_bits) {
    char error_buf[256];
    snprintf(error_buf, sizeof(error_buf),
             "Message too large for image! Available space = %zu bits, message "
             "size = %zu bits",
             available_bits, combined_bits);
    *error_message = strdup(error_buf);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return -1;
  }

  // Only open output file after validating message length
  outfile = fopen(output_path, "wb");
  if (!outfile) {
    perror("Failed to open output file");
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return -1;
  }

  dstinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&dstinfo);
  jpeg_stdio_dest(&dstinfo, outfile);
  jpeg_copy_critical_parameters(&cinfo, &dstinfo);

  for (JDIMENSION row = 0; row < height_in_blocks; row++) {
    JBLOCKARRAY row_ptrs = (cinfo.mem->access_virt_barray)(
        (j_common_ptr)&cinfo, coef_arrays[0], row, 1, TRUE);

    embed_message(row_ptrs, width_in_blocks, combined_message, &bit_index,
                  combined_bits);
    if (bit_index >= combined_bits)
      break;
  }

  printf("writing coefficients\n");
  jpeg_write_coefficients(&dstinfo, coef_arrays);
  jpeg_finish_compress(&dstinfo);
  jpeg_destroy_compress(&dstinfo);

  printf("finishing decompress\n");
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  fclose(infile);
  fclose(outfile);
  printf("closing files\n");

  return 0;
}

char *extract_message_from_jpeg(const char *input_path, const char *header,
                                char **error_message) {
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *infile = NULL;
  jvirt_barray_ptr *coef_arrays;

  infile = fopen(input_path, "rb");
  if (!infile) {
    *error_message = strdup("Failed to open input file");
    return NULL;
  }

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  coef_arrays = jpeg_read_coefficients(&cinfo);

  char *message = NULL;
  jpeg_component_info *comp = &cinfo.comp_info[0];
  JDIMENSION width_in_blocks = comp->width_in_blocks;
  JDIMENSION height_in_blocks = comp->height_in_blocks;

  printf("width_in_blocks: %d\n", width_in_blocks);
  printf("height_in_blocks: %d\n", height_in_blocks);
  printf("header: %s\n", header);

  for (JDIMENSION row = 0; row < height_in_blocks; row++) {
    JBLOCKARRAY row_ptrs = (cinfo.mem->access_virt_barray)(
        (j_common_ptr)&cinfo, coef_arrays[0], row, 1, FALSE);

    message = extract_message(row_ptrs, width_in_blocks, height_in_blocks,
                              header, error_message);

    if (message != NULL && message[0] != '\0') {
      break;
    }
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);

  return message;
}