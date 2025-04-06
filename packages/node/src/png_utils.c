#include "png_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TERMINATOR 0xAA
#define MAX_BUFFER_SIZE 65536 // 64KB
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static void handle_png_error(png_structp png_ptr, png_const_charp error_msg) {
  fprintf(stderr, "PNG Error: %s\n", error_msg);
  longjmp(png_jmpbuf(png_ptr), 1);
}

int embed_message_in_png(const char *input_file, const char *output_file,
                         const char *message, const char *header,
                         char **error_message) {
  FILE *fp = fopen(input_file, "rb");
  if (!fp) {
    *error_message = strdup("Failed to open input file");
    return -1;
  }

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
                                           handle_png_error, NULL);
  if (!png) {
    fclose(fp);
    *error_message = strdup("Failed to create read struct");
    return -1;
  }

  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_read_struct(&png, NULL, NULL);
    fclose(fp);
    *error_message = strdup("Failed to create info struct");
    return -1;
  }

  if (setjmp(png_jmpbuf(png))) {
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
    *error_message = strdup("Failed to setjmp");
    return -1;
  }

  png_init_io(png, fp);
  png_read_info(png, info);

  int width = png_get_image_width(png, info);
  int height = png_get_image_height(png, info);
  int color_type = png_get_color_type(png, info);
  int bit_depth = png_get_bit_depth(png, info);

  printf("width: %d\n", width);
  printf("height: %d\n", height);
  printf("color_type: %d\n", color_type);
  printf("bit_depth: %d\n", bit_depth);

  // Convert to RGBA if necessary
  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);
  if (png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  if (bit_depth == 16)
    png_set_strip_16(png);
  if (color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  if (color_type == PNG_COLOR_TYPE_RGB) {
    png_set_filler(png, 0xff, PNG_FILLER_AFTER);
  }
  png_read_update_info(png, info);

  png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
  for (int y = 0; y < height; y++) {
    row_pointers[y] = (png_bytep)malloc(png_get_rowbytes(png, info));
  }

  png_read_image(png, row_pointers);
  fclose(fp);

  printf("header: %s\n", header);
  printf("embed_message: %s\n", message);

  char combined_message[strlen(header) + strlen(message) +
                        2]; // +2 for terminator and null
  strcpy(combined_message, header);
  strcat(combined_message, message);

  char terminator = TERMINATOR;
  strncat(combined_message, &terminator, 1);

  size_t combined_length = strlen(combined_message);
  size_t required_bits = combined_length * 8;
  size_t available_bits = width * height * 4;

  printf("available_bits: %zu\n", available_bits);
  printf("combined_length: %zu bytes (%zu bits)\n", combined_length,
         required_bits);

  if (required_bits > available_bits) {
    char error_buf[256];
    snprintf(error_buf, sizeof(error_buf),
             "Message too large for image! Available space = %zu bits, message "
             "size = %zu bits",
             available_bits, required_bits);
    *error_message = strdup(error_buf);
    return -1;
  }

  size_t bit_index = 0;

  for (int y = 0; y < height && bit_index < required_bits; y++) {
    for (int x = 0; x < width && bit_index < required_bits; x++) {
      png_bytep px = &(row_pointers[y][x * 4]); // RGBA format

      int i = bit_index / 8;
      int j = 7 - (bit_index % 8);
      int bit = (combined_message[i] >> j) & 1;

      // alternate between channels
      int channel = bit_index % 4;
      px[channel] =
          (px[channel] & ~1) | bit; // Modify R/G/B/A channel based on bit_index

      bit_index++;
    }
  }

  fp = fopen(output_file, "wb");
  png_structp png_write =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info_write = png_create_info_struct(png_write);
  png_init_io(png_write, fp);

  png_set_IHDR(png_write, info_write, width, height, 8, PNG_COLOR_TYPE_RGBA,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);

  png_write_info(png_write, info_write);
  png_write_image(png_write, row_pointers);
  png_write_end(png_write, NULL);

  fclose(fp);
  png_destroy_write_struct(&png_write, &info_write);

  return 0;
}

char *extract_message_from_png(const char *input_file, const char *header,
                               char **error_message) {
  FILE *fp = fopen(input_file, "rb");
  if (!fp) {
    *error_message = strdup("Failed to open input file");
    return NULL;
  }

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
                                           handle_png_error, NULL);
  if (!png) {
    fclose(fp);
    *error_message = strdup("Failed to create read struct");
    return NULL;
  }

  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_read_struct(&png, NULL, NULL);
    fclose(fp);
    *error_message = strdup("Failed to create info struct");
    return NULL;
  }

  if (setjmp(png_jmpbuf(png))) {
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
    *error_message = strdup("Failed to setjmp");
    return NULL;
  }

  png_init_io(png, fp);
  png_read_info(png, info);

  int width = png_get_image_width(png, info);
  int height = png_get_image_height(png, info);
  int color_type = png_get_color_type(png, info);
  int bit_depth = png_get_bit_depth(png, info);

  printf("width: %d\n", width);

  // Convert to RGBA if necessary
  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);
  if (png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  if (bit_depth == 16)
    png_set_strip_16(png);
  if (color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  if (color_type == PNG_COLOR_TYPE_RGB) {
    png_set_filler(png, 0xff, PNG_FILLER_AFTER);
  }
  png_read_update_info(png, info);

  png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
  for (int y = 0; y < height; y++) {
    row_pointers[y] = (png_bytep)malloc(png_get_rowbytes(png, info));
  }

  png_read_image(png, row_pointers);
  fclose(fp);

  unsigned char current_byte = 0;
  size_t bit_index = 0;
  size_t bits_collected = 0;
  size_t message_length = 0;
  int done = 0;
  int header_found = 0;
  size_t header_length = strlen(header);

  size_t available_bits = width * height * 8; // 1 bit per alpha channel value
  size_t max_message_length = available_bits / 8;

  printf("available_bits: %zu\n", available_bits);
  printf("max_message_length: %zu\n", max_message_length);
  printf("header_length: %zu\n", header_length);

  char *buffer = (char *)malloc(MIN(max_message_length, MAX_BUFFER_SIZE));
  if (buffer == NULL) {
    *error_message = strdup("Memory allocation failed");
    for (int y = 0; y < height; y++) {
      free(row_pointers[y]);
    }
    free(row_pointers);
    return NULL;
  }

  for (int y = 0; y < height && !done; y++) {
    for (int x = 0; x < width && !done; x++) {
      png_bytep px = &(row_pointers[y][x * 4]);

      // alternate between channels
      int channel = bit_index % 4;
      int bit = px[channel] & 1;

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

          if (!header_found && header_length > 0 &&
              message_length >= header_length) {
            if (strncmp(buffer + message_length - header_length, header,
                        header_length) == 0) {
              header_found = 1;
              printf("Header found — resetting message_length\n");
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
      bit_index++;
    }
  }

  for (int y = 0; y < height; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);

  if (!header_found && header_length > 0) {
    printf("Header not found — discarding extracted message\n");
    free(buffer);
    return NULL;
  }

  return buffer;
}