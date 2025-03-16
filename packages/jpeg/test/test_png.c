#include "png_utils.h"
#include "unity.h"
#include <stdio.h>
#define FIXTURE_DIR "test/fixtures/"

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
  const char *input = getFixturePath("test.png");
  // Check file exists
  printf("Input file: %s\n", input);
  FILE *file = fopen(input, "rb");
  TEST_ASSERT_NOT_NULL(file);
  fclose(file);

  const char *output = getFixturePath("output.png");
  const char *message = "Hello, Stego!";
  const char *header = "STEGO";

  int embed_result = embed_message_in_png(input, output, header, message);
  TEST_ASSERT_EQUAL(0, embed_result);

  int extract_result = extract_message_from_png(output, header);
  TEST_ASSERT_EQUAL(0, extract_result);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(testPngBasicMessage);
  return UNITY_END();
}