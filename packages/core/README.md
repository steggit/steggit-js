# @steggit-js/core

Core C implementation of Steggit's steganography functionality. This package provides the fundamental steganography operations that power both the browser and Node.js implementations. It can also be used as a standalone C module.

## Features

- LSB (Least Significant Bit) steganography for PNG and JPEG images
- High-performance C implementation
- Support for both encoding and decoding operations
- Custom header support for message identification
- Memory-safe operations with proper error handling
- Comprehensive test suite

## Usage

### As a C Module

```c
#include "steggit.h"

int main() {
    // Initialize the steganography context
    StegoContext ctx;
    if (stego_init(&ctx) != STEGO_OK) {
        // Handle error
        return 1;
    }

    // Encode a message into an image
    const char* input_path = "input.png";
    const char* output_path = "output.png";
    const char* message = "Secret message";
    const char* header = "MY_HEADER";

    if (stego_encode_png(&ctx, input_path, output_path, message, header) != STEGO_OK) {
        // Handle error
        stego_cleanup(&ctx);
        return 1;
    }

    // Decode a message from an image
    char decoded_message[1024];
    if (stego_decode_png(&ctx, output_path, decoded_message, sizeof(decoded_message), header) != STEGO_OK) {
        // Handle error
        stego_cleanup(&ctx);
        return 1;
    }

    // Clean up
    stego_cleanup(&ctx);
    return 0;
}
```

### Building

#### Prerequisites

- CMake 3.10 or later
- libjpeg-turbo
- libpng
- C compiler (GCC or Clang)

#### Build Steps

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
make

# Run tests
./test_runner
```

### Integration with Other Packages

This core module is used by:
- `@steggit-js/browser`: Browser implementation using WebAssembly
- `@steggit-js/node`: Node.js native addon implementation

## API Reference

### Command Line Interface

The core module can be used directly from the command line with the following syntax:

```bash
# Encode a message into an image
./stego encode --input <input_path> --output <output_path> --message <message> --header <header>

# Decode a message from an image
./stego decode --input <input_path> --header <header>
```

#### Parameters

- `--input <input_path>`: Path to the input image file (required)
  - For encoding: The source image to embed the message in
  - For decoding: The image containing the hidden message
  - Supported formats: PNG, JPEG

- `--output <output_path>`: Path to save the output image (required for encoding only)
  - The resulting image with the embedded message
  - Must be a different file from the input
  - Format will match the input format

- `--message <message>`: The message to encode (required for encoding only)
  - The text to hide in the image
  - Maximum length depends on image size and format
  - Special characters are supported

- `--header <header>`: Custom header for message identification (optional)
  - Used to identify and validate the hidden message
  - Must match between encode and decode operations
  - Defaults to a standard header if not specified


## Development

### Project Structure

```
core/
├── include/         # Public header files
├── src/            # Source files
│   ├── main.c      # Main implementation
│   ├── png_utils.c # PNG-specific utilities
│   ├── jpeg_utils.c # JPEG-specific utilities
│   └── general_utils.c # General utilities
├── test/           # Test files
├── third_party/    # Third-party dependencies
└── assets/         # Test assets
```

### Testing

The project uses Unity for testing. Run tests with:

```bash
make test
# or
./build/test_runner
```

### Memory Safety

The implementation includes:
- Address Sanitizer (ASan) for debugging
- Proper memory management
- Input validation
- Error handling

## License

This package is part of the Steggit monorepo and is licensed under the MIT License.
