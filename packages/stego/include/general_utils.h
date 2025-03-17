#ifndef GENERAL_UTILS_H
#define GENERAL_UTILS_H

struct Config {
  char *mode;
  char *input;
  char *output;
  char *message;
  char *header;
};

const char *get_mime_type(const char *filename);

int parse_args(int argc, char *argv[], struct Config *config);

#endif // GENERAL_UTILS_H