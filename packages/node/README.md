# @steggit/node

Node.js native addon for image steganography, providing high-performance encoding and decoding of text messages within PNG and JPEG images. This package uses the core C implementation through Node.js native addons.

## Features

- Encode text messages into PNG images using LSB steganography
- Encode text messages into JPEG images using DCT steganography
- Decode text messages from PNG and JPEG images
- Custom header support for message identification
- Native performance through C bindings
- TypeScript support with full type definitions
- Both CommonJS and ES Module support

## Installation

```bash
npm install @steggit/node
# or
yarn add @steggit/node
```

### Prerequisites

- Node.js 20 or later
- Python 3.x (for node-gyp)
- C++ compiler (GCC or Clang)
- libjpeg-turbo
- libpng

## Usage

### Basic Usage

```javascript
import { encodeTextPng, decodeTextPng } from '@steggit/node';

// Encoding a message into an image
async function encodeMessage() {
  try {
    await encodeTextPng(
      'input.png',
      'output.png',
      'This is a secret message'
    );
    console.log('Message encoded successfully');
  } catch (error) {
    console.error('Error encoding message:', error);
  }
}

// Decoding a message from an image
async function decodeMessage() {
  try {
    const message = await decodeTextPng('output.png');
    console.log('Decoded message:', message);
  } catch (error) {
    console.error('Error decoding message:', error);
  }
}
```

### Advanced Usage with Custom Headers

```javascript
import { encodeTextJpeg, decodeTextJpeg } from '@steggit/node';

async function encodeWithHeader() {
  try {
    await encodeTextJpeg(
      'input.jpg',
      'output.jpg',
      'This is a secret message',
      'MY_APP_HEADER'
    );
    console.log('Message encoded successfully');
  } catch (error) {
    console.error('Error encoding message:', error);
  }
}

async function decodeWithHeader() {
  try {
    const message = await decodeTextJpeg(
      'output.jpg',
      'MY_APP_HEADER'
    );
    console.log('Decoded message:', message);
  } catch (error) {
    console.error('Error decoding message:', error);
  }
}
```

## Development

### Building

```bash
# Install dependencies
npm install

# Build the native addon
npm run build
```

### Testing

```bash
npm test
```

## License

This package is part of the Steggit monorepo and is licensed under the MIT License.
