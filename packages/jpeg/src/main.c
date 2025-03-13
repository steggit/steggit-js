#include <stdio.h>
#include <jpeglib.h>
#include <string.h>

#define TERMINATOR 0xFF // 8-bit terminator to signal end of message

void embed_message(JBLOCKARRAY row_ptrs, JDIMENSION width_in_blocks,
                   const char *message, size_t *bit_index,
                   size_t message_length) {
  for (JDIMENSION col = 0; col < width_in_blocks; col++) {
    JCOEF *block = row_ptrs[0][col];
    for (int i = 1; i < DCTSIZE2; i++) {
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

int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("Usage: %s <input.jpg> <output.jpg> <message>\n", argv[0]);
    return 1;
  }

  const char *message = argv[3];

  FILE *infile = fopen(argv[1], "rb");
  if (!infile) {
    perror("Error opening input file");
    return 1;
  }

  FILE *outfile = fopen(argv[2], "wb");
  if (!outfile) {
    perror("Error opening output file");
    fclose(infile);
    return 1;
  }

  struct jpeg_decompress_struct cinfo;
  struct jpeg_compress_struct dstinfo;
  struct jpeg_error_mgr jerr;

  memset(&jerr, 0, sizeof(jerr));
  memset(&cinfo, 0, sizeof(cinfo));
  memset(&dstinfo, 0, sizeof(dstinfo));

  cinfo.err = jpeg_std_error(&jerr);
  dstinfo.err = jpeg_std_error(&jerr);

  // Initialize decompression
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  if (jpeg_read_header(&cinfo, TRUE) != JPEG_HEADER_OK) {
    fprintf(stderr, "Error reading JPEG header\n");
    goto cleanup;
  }

  // Read coefficients
  jvirt_barray_ptr *coef_arrays = jpeg_read_coefficients(&cinfo);
  if (!coef_arrays) {
    fprintf(stderr, "Failed to read coefficients\n");
    goto cleanup;
  }

  // Initialize compression
  jpeg_create_compress(&dstinfo);
  jpeg_stdio_dest(&dstinfo, outfile);
  jpeg_copy_critical_parameters(&cinfo, &dstinfo);

  // Stego: modify the DCT coefficients
  jpeg_component_info *comp = &cinfo.comp_info[0];
  JDIMENSION width_in_blocks = comp->width_in_blocks;
  JDIMENSION height_in_blocks = comp->height_in_blocks;

  printf("Height in blocks: %d\n", height_in_blocks);
  printf("Width in blocks: %d\n", width_in_blocks);

  size_t bit_index = 0;                        // Track bits across all rows
  size_t message_length = strlen(message) * 8; // Total bits in message

  for (JDIMENSION row = 0; row < height_in_blocks; row++) {
    JBLOCKARRAY row_ptrs = (cinfo.mem->access_virt_barray)(
        (j_common_ptr)&cinfo, coef_arrays[0], row, 1, TRUE);

    embed_message(row_ptrs, width_in_blocks, message, &bit_index,
                  message_length);

    if (bit_index >= message_length + 8) {
      break;
    }
  }

  printf("Writing coefficients\n");
  jpeg_write_coefficients(&dstinfo, coef_arrays);
  printf("Finishing compress\n");
  jpeg_finish_compress(&dstinfo);
  jpeg_destroy_compress(&dstinfo);
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  fclose(infile);
  fclose(outfile);

  printf("Stego complete! Output written to %s\n", argv[2]);
  return 0;

cleanup:
  jpeg_destroy_decompress(&cinfo);
  jpeg_destroy_compress(&dstinfo);
  fclose(infile);
  fclose(outfile);
  return 1;
}