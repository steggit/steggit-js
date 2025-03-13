#include <stdio.h>
#include <jpeglib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input.jpg>\n", argv[0]);
        return 1;
    }

    // Open the input JPEG file
    FILE *infile = fopen(argv[1], "rb");
    if (!infile) {
        perror("Error opening file");
        return 1;
    }

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);

    // Read file header
    jpeg_read_header(&cinfo, TRUE);
    printf("Width: %d\n", cinfo.image_width);
    printf("Height: %d\n", cinfo.image_height);
    printf("Color Components: %d\n", cinfo.num_components);

    jpeg_destroy_decompress(&cinfo);
    fclose(infile);

    return 0;
}