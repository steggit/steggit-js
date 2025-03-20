#include "general_utils.h"
#include "jpeg_utils.h"
#include "png_utils.h"
#include "types.h"
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
  // Check files exist
  FILE *f1 = fopen("test/fixtures/test.png", "rb");
  TEST_ASSERT_NOT_NULL(f1);
  fclose(f1);

  FILE *f2 = fopen("test/fixtures/test.jpg", "rb");
  TEST_ASSERT_NOT_NULL(f2);
  fclose(f2);

  FILE *f3 = fopen("test/fixtures/tiny_test.png", "rb");
  TEST_ASSERT_NOT_NULL(f3);
  fclose(f3);

  FILE *f4 = fopen("test/fixtures/tiny_test.jpg", "rb");
  TEST_ASSERT_NOT_NULL(f4);
  fclose(f4);

  FILE *f5 = fopen("test/fixtures/transparent_test.png", "rb");
  TEST_ASSERT_NOT_NULL(f5);
  fclose(f5);

  printf("All files exist\n");
}

void tearDown(void) {
  // Optional: Called after every test
}

void testInvalidMode(void) {
  const char *argv[] = {"stego", "invalid", "--input",
                        "test/fixtures/test.png"};
  int argc = sizeof(argv) / sizeof(argv[0]);
  Config config = {0};
  int result = parse_args(argc, (char **)argv, &config);
  TEST_ASSERT_EQUAL(1, result);
}

void testInvalidEncodeArgs(void) {
  const char *argv[] = {"stego", "encode", "--input", "test/fixtures/test.png"};
  int argc = sizeof(argv) / sizeof(argv[0]);
  Config config = {0};
  int result = parse_args(argc, (char **)argv, &config);
  TEST_ASSERT_EQUAL(1, result);
}

void testInvalidDecodeArgs(void) {
  const char *argv[] = {"stego", "decode", "--output", "test/output/test.png"};
  int argc = sizeof(argv) / sizeof(argv[0]);
  Config config = {0};
  int result = parse_args(argc, (char **)argv, &config);
  TEST_ASSERT_EQUAL(1, result);
}

void testValidEncodeArgs(void) {
  const char *argv[] = {"stego",     "encode",
                        "--input",   "test/fixtures/test.png",
                        "--output",  "test/output/test.png",
                        "--message", "This is a basic message in a PNG file!",
                        "--header",  "$$"};
  int argc = sizeof(argv) / sizeof(argv[0]);
  Config config = {0};
  int result = parse_args(argc, (char **)argv, &config);
  TEST_ASSERT_EQUAL(0, result);
  TEST_ASSERT_EQUAL_STRING("encode", config.mode);
  TEST_ASSERT_EQUAL_STRING("test/fixtures/test.png", config.input);
  TEST_ASSERT_EQUAL_STRING("test/output/test.png", config.output);
  TEST_ASSERT_EQUAL_STRING("This is a basic message in a PNG file!",
                           config.message);
}

void testValidDecodeArgs(void) {
  const char *argv[] = {"stego", "decode", "--input", "test/fixtures/test.png"};
  int argc = sizeof(argv) / sizeof(argv[0]);
  Config config = {0};
  int result = parse_args(argc, (char **)argv, &config);
  TEST_ASSERT_EQUAL(0, result);
  TEST_ASSERT_EQUAL_STRING("decode", config.mode);
  TEST_ASSERT_EQUAL_STRING("test/fixtures/test.png", config.input);
}

void testMimetype(void) {
  const char *input = "test/fixtures/test.png";
  const char *mimetype = get_mime_type(input);
  TEST_ASSERT_EQUAL_STRING("image/png", mimetype);

  input = "test/fixtures/test.jpg";
  mimetype = get_mime_type(input);
  TEST_ASSERT_EQUAL_STRING("image/jpeg", mimetype);

  input = "test/fixtures/test.txt";
  mimetype = get_mime_type(input);
  TEST_ASSERT_NULL(mimetype);
}

void testPngBasicMessage(void) {
  const char *input = "test/fixtures/test.png";
  const char *output = "test/output/basic_message.png";
  const char *message = "This is a basic message in a PNG file!";
  const char *header = "$$";
  char *error_message = NULL;

  int embed_result =
      embed_message_in_png(input, output, message, header, &error_message);
  TEST_ASSERT_EQUAL(0, embed_result);

  char *extract_result =
      extract_message_from_png(output, header, &error_message);
  TEST_ASSERT_EQUAL_STRING(message, extract_result);
  free(extract_result);
}

void testJpegBasicMessage(void) {
  const char *input = "test/fixtures/test.jpg";
  const char *output = "test/output/basic_message.jpg";
  const char *message = "This is a simple JPEG test!";
  const char *header = "$$";
  char *error_message = NULL;
  int embed_result =
      embed_message_in_jpeg(input, output, message, header, &error_message);
  TEST_ASSERT_EQUAL(0, embed_result);

  char *extract_result =
      extract_message_from_jpeg(output, header, &error_message);
  TEST_ASSERT_EQUAL_STRING(message, extract_result);
  free(extract_result);
}

void testPngCustomHeader(void) {
  const char *input = "test/fixtures/test.png";
  const char *output = "test/output/custom_header.png";
  const char *message = "This is a different message";
  const char *header = "CUSTOM";
  char *error_message = NULL;

  int embed_result =
      embed_message_in_png(input, output, message, header, &error_message);
  TEST_ASSERT_EQUAL(0, embed_result);

  char *extract_result =
      extract_message_from_png(output, header, &error_message);
  TEST_ASSERT_EQUAL_STRING(message, extract_result);
  free(extract_result);

  // Test with incorrect header
  char *extract_result_incorrect =
      extract_message_from_png(output, "WRONG", &error_message);
  TEST_ASSERT_NULL(extract_result_incorrect);
}

void testJpegCustomHeader(void) {
  const char *input = "test/fixtures/test.jpg";
  const char *output = "test/output/custom_header.jpg";
  const char *message = "This is a different message";
  const char *header = "CUSTOM";
  char *error_message = NULL;

  int embed_result =
      embed_message_in_jpeg(input, output, message, header, &error_message);
  TEST_ASSERT_EQUAL(0, embed_result);

  char *extract_result =
      extract_message_from_jpeg(output, header, &error_message);
  TEST_ASSERT_EQUAL_STRING(message, extract_result);
  free(extract_result);

  // Test with incorrect header
  char *extract_result_incorrect =
      extract_message_from_jpeg(output, "WRONG", &error_message);
  TEST_ASSERT_NULL(extract_result_incorrect);
}

void testTransparentPng(void) {
  const char *input = "test/fixtures/transparent_test.png";
  const char *output = "test/output/transparent_message.png";
  const char *message = "This is a message embedded in a transparent PNG!";
  const char *header = "$$";
  char *error_message = NULL;

  int embed_result =
      embed_message_in_png(input, output, message, header, &error_message);
  TEST_ASSERT_EQUAL(0, embed_result);

  char *extract_result =
      extract_message_from_png(output, header, &error_message);
  TEST_ASSERT_EQUAL_STRING(message, extract_result);
  free(extract_result);
}

void testTooLargeMessagePng(void) {
  const char *input = "test/fixtures/tiny_test.png";
  const char *output = "test/output/too_large_message.png";
  const char *message =
      "This is a message that is too large to embed in a PNG file. I have to "
      "add some more text to make it longer to ensure that the test fails.";
  const char *header = "$$";
  char *error_message = NULL;
  int embed_result =
      embed_message_in_png(input, output, message, header, &error_message);
  TEST_ASSERT_EQUAL(-1, embed_result);

  // Verify that the output file doesn't exist since embedding failed
  FILE *fp = fopen(output, "rb");
  TEST_ASSERT_NULL(fp);
  if (fp) {
    fclose(fp);
  }
}

void testTooLargeMessageJpeg(void) {
  const char *input = "test/fixtures/tiny_test.jpg";
  const char *output = "test/output/too_large_message.jpg";
  const char *message =
      "This is a message that is too large to embed in a JPEG file. I have to "
      "add some more text to make it longer to ensure that the test fails.";
  const char *header = "$$";
  char *error_message = NULL;

  int embed_result =
      embed_message_in_jpeg(input, output, message, header, &error_message);
  TEST_ASSERT_EQUAL(-1, embed_result);

  // Verify that the output file doesn't exist since embedding failed
  FILE *fp = fopen(output, "rb");
  TEST_ASSERT_NULL(fp);
  if (fp) {
    fclose(fp);
  }
}

int main(void) {
  create_output_dir();
  atexit(cleanup);

  UNITY_BEGIN();
  RUN_TEST(testInvalidMode);
  RUN_TEST(testInvalidEncodeArgs);
  RUN_TEST(testInvalidDecodeArgs);
  RUN_TEST(testValidEncodeArgs);
  RUN_TEST(testValidDecodeArgs);
  RUN_TEST(testMimetype);
  RUN_TEST(testPngBasicMessage);
  RUN_TEST(testJpegBasicMessage);
  RUN_TEST(testPngCustomHeader);
  RUN_TEST(testJpegCustomHeader);
  RUN_TEST(testTransparentPng);
  RUN_TEST(testTooLargeMessagePng);
  RUN_TEST(testTooLargeMessageJpeg);
  return UNITY_END();
}