#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <input.jpg> <output.jpg>\n", argv[0]);
        return 1;
    }

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

    // Initialize error handler
    memset(&jerr, 0, sizeof(jerr));
    memset(&cinfo, 0, sizeof(cinfo));
    memset(&dstinfo, 0, sizeof(dstinfo));
    
    cinfo.err = jpeg_std_error(&jerr);
    dstinfo.err = jpeg_std_error(&jerr);
    
    // Initialize decompression object
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);

    // Read the JPEG header
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

    // Initialize compression object
    jpeg_create_compress(&dstinfo);
    jpeg_stdio_dest(&dstinfo, outfile);
    
    // Copy parameters from input to output
    jpeg_copy_critical_parameters(&cinfo, &dstinfo);

    // Stego: modify the DCT coefficients
    JBLOCKARRAY row_ptrs;
    jpeg_component_info *comp = &cinfo.comp_info[0];
    JDIMENSION width_in_blocks = comp->width_in_blocks;
    JDIMENSION height_in_blocks = comp->height_in_blocks;

    printf("Height in blocks: %d\n", height_in_blocks);
    printf("Width in blocks: %d\n", width_in_blocks);

    for (JDIMENSION row = 0; row < height_in_blocks; row++) {
        row_ptrs = (cinfo.mem->access_virt_barray)
            ((j_common_ptr) &cinfo, coef_arrays[0], row, 1, TRUE);
        for (JDIMENSION col = 0; col < width_in_blocks; col++) {
            JCOEF *block = row_ptrs[0][col];
            // Skip DC (index 0), modify the LSB of first AC coefficient
            block[1] ^= 1;
        }
    }

    if (coef_arrays == NULL) {
        fprintf(stderr, "coef_arrays is NULL!\n");
        goto cleanup;
    }
    
    if (coef_arrays[0] == NULL) {
        fprintf(stderr, "coef_arrays[0] is NULL!\n");
        goto cleanup;
    }
    
    printf("Writing coefficients\n");
    jpeg_write_coefficients(&dstinfo, coef_arrays);
    printf("Finishing compress\n");
    jpeg_finish_compress(&dstinfo);
    printf("Destroying compress struct\n");
    jpeg_destroy_compress(&dstinfo);
    printf("Finishing decompress\n");
    jpeg_finish_decompress(&cinfo);
    printf("Destroying decompress struct\n");
    jpeg_destroy_decompress(&cinfo);

    fclose(infile);
    fclose(outfile);

    printf("Stego complete! Output written to %s\n", argv[2]);
    return 0;

cleanup:
    // Clean up in case of error
    jpeg_destroy_decompress(&cinfo);
    jpeg_destroy_compress(&dstinfo);
    fclose(infile);
    fclose(outfile);
    return 1;
}