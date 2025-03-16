#include "png_utils.h"
#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIXTURE_DIR "test/fixtures/"
#define OUTPUT_DIR "test/output/"

void cleanup(void) {
  printf("Cleaning up test output directory...\n");
  char command[256];
  snprintf(command, sizeof(command), "rm -rf %s", OUTPUT_DIR);
  int result = system(command);
  if (result != 0) {
    fprintf(stderr, "Failed to remove output directory\n");
  }
}

// Create output directory at the start
void create_output_dir(void) {
  printf("Creating output directory: %s\n", OUTPUT_DIR);
  // Check if directory already exists
  if (access(OUTPUT_DIR, F_OK) == 0) {
    // Directory exists, clear all files
    cleanup();
  } else {
    // Directory doesn't exist, create it
    if (mkdir(OUTPUT_DIR, 0777) != 0) {
      fprintf(stderr, "Failed to create output directory\n");
      exit(EXIT_FAILURE);
    }
  }
}

void setUp(void) {
  // Optional: Called before every test
}

void tearDown(void) {
  // Optional: Called after every test
}

// Utility function to get the path to a fixture
const char *getFixturePath(const char *filename) {
  static char path[256];
  snprintf(path, sizeof(path), "%s%s", FIXTURE_DIR, filename);
  return path;
}

void testPngBasicMessage(void) {
  const char *input = "test/fixtures/test.png";
  // Check file exists
  printf("Input file: %s\n", input);
  FILE *file = fopen(input, "rb");
  TEST_ASSERT_NOT_NULL(file);
  fclose(file);

  const char *output = "test/output/output.png";
  const char *message = "Hello, Stego!";
  const char *header = "STEGO";

  int embed_result = embed_message_in_png(input, output, header, message);
  TEST_ASSERT_EQUAL(0, embed_result);

  int extract_result = extract_message_from_png(output, header);
  TEST_ASSERT_EQUAL(0, extract_result);
}

int main(void) {
  create_output_dir();
  atexit(cleanup);

  UNITY_BEGIN();
  RUN_TEST(testPngBasicMessage);
  return UNITY_END();
}