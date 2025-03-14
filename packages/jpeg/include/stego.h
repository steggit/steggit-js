#ifndef STEGO_H
#define STEGO_H

#include <_stdio.h>
#include <jpeglib.h>

int embed_message_in_jpeg(const char *input_path, const char *output_path, const char *message);
int extract_message_from_jpeg(const char *input_path);

#endif // STEGO_H