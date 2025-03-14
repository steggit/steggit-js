#include "stego.h"
#include <stdio.h>

int main(int argc, char *argv[]) {

  if (argc == 2) {
    // Decode Mode
    if (extract_message_from_jpeg(argv[1]) != 0) {
      fprintf(stderr, "Failed to extract message\n");
      return 1;
    }
  } else if (argc == 4) {
    // Encode Mode
    if (embed_message_in_jpeg(argv[1], argv[2], argv[3]) != 0) {
      fprintf(stderr, "Failed to embed message\n");
      return 1;
    }
  } else {
    printf("Usage:\n");
    printf("  Encode: %s <input.jpg> <output.jpg> <message>\n", argv[0]);
    printf("  Decode: %s <input.jpg>\n", argv[0]);
    return 1;
  }

  return 0;
}