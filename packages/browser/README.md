# @steggit/browser

Browser-specific implementation of Steggit, providing steganography functionality for web applications. This package allows you to encode and decode text messages within PNG and JPEG images directly in the browser.

## Features

- Encode text messages into PNG images
- Encode text messages into JPEG images
- Decode text messages from PNG images
- Decode text messages from JPEG images
- Custom header support for message identification
- WebAssembly-based implementation for optimal performance
- TypeScript support with full type definitions

## Installation

```bash
npm install @steggit/browser
# or
yarn add @steggit/browser
```

## Usage

### Basic Usage

```typescript
import { encodeTextPng, decodeTextPng } from '@steggit/browser';

// Encoding a message into an image
async function encodeMessage() {
  const imageFile = document.querySelector('input[type="file"]').files[0];
  const message = 'This is a secret message';
  
  try {
    const encodedImage = await encodeTextPng(imageFile, message);
    // Use the encoded image blob
    const url = URL.createObjectURL(encodedImage);
    const img = document.createElement('img');
    img.src = url;
    document.body.appendChild(img);
  } catch (error) {
    console.error('Error encoding message:', error);
  }
}

// Decoding a message from an image
async function decodeMessage() {
  const imageFile = document.querySelector('input[type="file"]').files[0];
  
  try {
    const decodedMessage = await decodeTextPng(imageFile);
    console.log('Decoded message:', decodedMessage);
  } catch (error) {
    console.error('Error decoding message:', error);
  }
}
```

### Advanced Usage with Custom Headers

```typescript
import { encodeTextJpeg, decodeTextJpeg } from '@steggit/browser';

async function encodeWithHeader() {
  const imageFile = document.querySelector('input[type="file"]').files[0];
  const message = 'This is a secret message';
  const customHeader = 'MY_APP_HEADER';
  
  try {
    const encodedImage = await encodeTextJpeg(imageFile, message, customHeader);
    // Use the encoded image
  } catch (error) {
    console.error('Error encoding message:', error);
  }
}

async function decodeWithHeader() {
  const imageFile = document.querySelector('input[type="file"]').files[0];
  const customHeader = 'MY_APP_HEADER';
  
  try {
    const decodedMessage = await decodeTextJpeg(imageFile, customHeader);
    console.log('Decoded message:', decodedMessage);
  } catch (error) {
    console.error('Error decoding message:', error);
  }
}
```

## API Reference

### Functions

- `encodeTextPng(input: File | Buffer | string, message: string, header?: string): Promise<Blob>`
  - Encodes a text message into a PNG image
  - Returns a Blob containing the encoded image

- `encodeTextJpeg(input: File | Buffer | string, message: string, header?: string): Promise<Blob>`
  - Encodes a text message into a JPEG image
  - Returns a Blob containing the encoded image

- `decodeTextPng(input: File | Buffer | string, header?: string): Promise<string>`
  - Decodes a text message from a PNG image
  - Returns the decoded message as a string

- `decodeTextJpeg(input: File | Buffer | string, header?: string): Promise<string>`
  - Decodes a text message from a JPEG image
  - Returns the decoded message as a string

### Input Types

The input parameter can be one of:
- `File`: A browser File object (from file input)
- `Buffer`: A Node.js Buffer (for server-side usage)
- `string`: A data URL

## Development

### Building

```bash
# Build the WebAssembly module and bundle the package
yarn build

# Build only the WebAssembly module
yarn build:wasm

# Bundle the package
yarn bundle
```

### Testing

```bash
yarn test
```

## Browser Support

This package uses WebAssembly and requires a modern browser with WebAssembly support:
- Chrome 57+
- Firefox 52+
- Safari 11+
- Edge 16+

## License

This package is part of the Steggit monorepo and is licensed under the MIT License.
