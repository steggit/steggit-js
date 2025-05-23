#ifndef PNG_UTILS_H
#define PNG_UTILS_H

#include <png.h>

int embed_message_in_png(const char *input_file, const char *output_file,
                         const char *message, const char *header,
                         char **error_message);
char *extract_message_from_png(const char *input_file, const char *header,
                               char **error_message);

#endif // PNG_UTILS_H