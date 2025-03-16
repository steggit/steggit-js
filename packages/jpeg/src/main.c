#include "stego.h"
#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[]) {

  if (argc < 3) {
    printf("Usage:\n");
    printf("  Encode: %s encode <input.jpg> <output.jpg> <message>\n", argv[0]);
    printf("  Decode: %s decode <input.jpg>\n", argv[0]);
    return 1;
  }

  if (strcmp(argv[1], "decode") == 0) {
    // Decode Mode
    if (argc != 3) {
      printf("Decode usage: %s decode <input.jpg>\n", argv[0]);
      return 1;
    }
    if (extract_message_from_jpeg(argv[2]) != 0) {
      fprintf(stderr, "Failed to extract message\n");
      return 1;
    }
  } else if (strcmp(argv[1], "encode") == 0) {
    // Encode Mode
    if (argc != 5) {
      printf("Encode usage: %s encode <input.jpg> <output.jpg> <message>\n", argv[0]);
      return 1;
    }
    if (embed_message_in_jpeg(argv[2], argv[3], argv[4]) != 0) {
      fprintf(stderr, "Failed to embed message\n");
      return 1;
    }
  } else {
    printf("Unknown command: %s\n", argv[1]);
    printf("Usage:\n");
    printf("  Encode: %s encode <input.jpg> <output.jpg> <message>\n", argv[0]);
    printf("  Decode: %s decode <input.jpg>\n", argv[0]);
    return 1;
  }

  return 0;
}