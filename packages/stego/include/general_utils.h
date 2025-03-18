#ifndef GENERAL_UTILS_H
#define GENERAL_UTILS_H

#include "types.h"

const char *get_mime_type(const char *filename);

int parse_args(int argc, char *argv[], Config *config);

#endif // GENERAL_UTILS_H