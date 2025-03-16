#include "stego.h"
#include "jpeg_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TERMINATOR 0xFF

int embed_message_in_jpeg(const char *input_path, const char *output_path,
                          const char *message) {
  struct jpeg_decompress_struct cinfo;
  struct jpeg_compress_struct dstinfo;
  struct jpeg_error_mgr jerr;
  FILE *infile = NULL, *outfile = NULL;
  jvirt_barray_ptr *coef_arrays;

  infile = fopen(input_path, "rb");
  if (!infile) {
    perror("Failed to open input file");
    return -1;
  }

  outfile = fopen(output_path, "wb");
  if (!outfile) {
    perror("Failed to open output file");
    fclose(infile);
    return -1;
  }

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  coef_arrays = jpeg_read_coefficients(&cinfo);

  dstinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&dstinfo);
  jpeg_stdio_dest(&dstinfo, outfile);
  jpeg_copy_critical_parameters(&cinfo, &dstinfo);

  size_t bit_index = 0;
  size_t message_length = strlen(message) * 8;

  jpeg_component_info *comp = &cinfo.comp_info[0];
  JDIMENSION width_in_blocks = comp->width_in_blocks;
  JDIMENSION height_in_blocks = comp->height_in_blocks;

  printf("width_in_blocks: %d\n", width_in_blocks);
  printf("height_in_blocks: %d\n", height_in_blocks);
  printf("embed_message: %s\n", message);

  size_t available_bits = (width_in_blocks * height_in_blocks) * 63;
  size_t available_bytes = available_bits / 8;

  printf("available_bytes: %zu\n", available_bytes);
  printf("message_length: %zu\n", message_length);

  if (message_length > available_bytes) {
    fprintf(stderr,
            "Error: Message too large! Max length = %zu bytes, but message "
            "length = %zu bytes\n",
            available_bytes, message_length);
    return EXIT_FAILURE;
  }

  for (JDIMENSION row = 0; row < height_in_blocks; row++) {
    JBLOCKARRAY row_ptrs = (cinfo.mem->access_virt_barray)(
        (j_common_ptr)&cinfo, coef_arrays[0], row, 1, TRUE);

    embed_message(row_ptrs, width_in_blocks, message, &bit_index,
                  message_length);
    if (bit_index >= message_length + 8)
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

  return EXIT_SUCCESS;
}

int extract_message_from_jpeg(const char *input_path) {
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *infile = NULL;
  jvirt_barray_ptr *coef_arrays;

  infile = fopen(input_path, "rb");
  if (!infile) {
    perror("Failed to open input file");
    return -1;
  }

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  coef_arrays = jpeg_read_coefficients(&cinfo);

  char message[1024] = {0};
  size_t bit_index = 0;

  jpeg_component_info *comp = &cinfo.comp_info[0];
  JDIMENSION width_in_blocks = comp->width_in_blocks;
  JDIMENSION height_in_blocks = comp->height_in_blocks;

  printf("width_in_blocks: %d\n", width_in_blocks);
  printf("height_in_blocks: %d\n", height_in_blocks);

  int done = 0;
  for (JDIMENSION row = 0; row < height_in_blocks && !done; row++) {
    JBLOCKARRAY row_ptrs = (cinfo.mem->access_virt_barray)(
        (j_common_ptr)&cinfo, coef_arrays[0], row, 1, FALSE);

    extract_message(row_ptrs, width_in_blocks, message, &bit_index,
                    sizeof(message));

    if (message[0] != '\0') {
      break;
    }
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);

  printf("Extracted message: \"%s\"\n", message);
  return EXIT_SUCCESS;
}