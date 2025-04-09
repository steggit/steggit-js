# @steggit-js/utils

A utility package for Steggit that provides encryption and string manipulation tools for steganography operations.

## Features

### Encryption Utilities
- Message encryption and decryption
- Encryption key generation
- Secure message parsing and stringification

### String Utilities
- String to binary conversion
- Binary to string conversion
- String obfuscation and deobfuscation
- Invisible character manipulation
- String compression

## Installation

```bash
npm install @steggit-js/utils
# or
yarn add @steggit-js/utils
```

## Usage

### Encryption

```typescript
import { 
  generateEncryptionKey,
  encryptMessage,
  decryptMessage,
  parseEncryptedMessage,
  stringifyEncryptedMessage
} from '@steggit-js/utils';

// Generate an encryption key
const key = await generateEncryptionKey({ secret, ctx, length });

// Encrypt a message
const encrypted = await encryptMessage('secret message', key);

// Convert to string for storage/transmission
const encryptedString = stringifyEncryptedMessage(encrypted);

// Parse back to encrypted message object
const parsedEncrypted = parseEncryptedMessage(encryptedString);

// Decrypt the message
const decrypted = await decryptMessage(parsedEncrypted, key);
```

### String Manipulation

```typescript
import {
  stringToBinary,
  binaryToString,
  obfuscateString,
  deobfuscateString,
  binaryToInvisible,
  invisibleToBinary
} from '@steggit-js/utils';

// Convert string to binary
const binary = stringToBinary('hello');

// Convert binary back to string
const text = binaryToString(binary);

// Obfuscate a string
const obfuscated = obfuscateString('secret message');

// Deobfuscate the string
const original = deobfuscateString(obfuscated);

// Convert to invisible characters
const invisible = binaryToInvisible(binary);

// Convert back from invisible characters
const visible = invisibleToBinary(invisible);
```

## Development

### Building

```bash
yarn build
```

### Testing

```bash
yarn test
```

## License

This package is part of the Steggit monorepo and is licensed under the MIT License.
