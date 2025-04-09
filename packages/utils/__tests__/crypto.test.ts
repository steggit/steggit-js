import {
  decryptMessage,
  encryptMessage,
  generateEncryptionKey,
  DEFAULT_KEY_LENGTH,
  stringifyEncryptedMessage,
  parseEncryptedMessage,
} from '../src/crypto/index';

const mockSecret = '0987654321';

describe(generateEncryptionKey.name, () => {
  it('should return an encryption key', () => {
    const key = generateEncryptionKey({ secret: mockSecret });
    expect(key.length).toBe(DEFAULT_KEY_LENGTH * 2);
  });

  it('should return the same key with the same input', () => {
    const key1 = generateEncryptionKey({ secret: mockSecret });
    const key2 = generateEncryptionKey({ secret: mockSecret });
    expect(key1).toBe(key2);
  });

  it('should return a different key with different input', () => {
    const key1 = generateEncryptionKey({ secret: mockSecret });
    const key2 = generateEncryptionKey({ secret: 'differentsecret' });
    expect(key1).not.toBe(key2);
  });
});

describe(encryptMessage.name, () => {
  it('should throw if key is too short', () => {
    const message = 'test';
    const key = generateEncryptionKey({ secret: mockSecret, length: 16 });
    expect(() => encryptMessage(message, key)).toThrow('Invalid key length');
  });

  it('should encrypt a message', () => {
    const message = 'testtesttest';
    const key = generateEncryptionKey({ secret: mockSecret });
    const encrypted = encryptMessage(message, key);
    expect(encrypted).toBeDefined();
    expect(encrypted.iv.length).toBe(24);
    expect(encrypted.content.length).toBe(24);
    expect(encrypted.authTag.length).toBe(32);
    expect(encrypted.content).not.toBe(message);
  });

  it('the same message should be encrypted differently each time', () => {
    const message = 'test';
    const key = generateEncryptionKey({ secret: mockSecret });
    const encrypted1 = encryptMessage(message, key);
    const encrypted2 = encryptMessage(message, key);
    expect(encrypted1).not.toBe(encrypted2);
  });
});

describe(decryptMessage.name, () => {
  it('should throw an error if key is incorrect', () => {
    const message = 'test';
    const key = generateEncryptionKey({ secret: mockSecret });
    const encrypted = encryptMessage(message, key);
    const wrongKey = generateEncryptionKey({ secret: 'wrongsecret' });
    expect(() => decryptMessage(encrypted, wrongKey)).toThrow(
      'Unsupported state or unable to authenticate data',
    );
  });

  it('should decrypt a message', () => {
    const message = 'test';
    const key = generateEncryptionKey({ secret: mockSecret });
    const encrypted = encryptMessage(message, key);
    const decrypted = decryptMessage(encrypted, key);
    expect(decrypted).toBe(message);
    expect(decrypted).not.toBe(encrypted.content);
  });

  it('the same message should be decrypted the same each time', () => {
    const message = 'test';
    const key = generateEncryptionKey({ secret: mockSecret });
    const encrypted1 = encryptMessage(message, key);
    const encrypted2 = encryptMessage(message, key);
    const decrypted1 = decryptMessage(encrypted1, key);
    const decrypted2 = decryptMessage(encrypted2, key);
    expect(decrypted1).toBe(message);
    expect(decrypted2).toBe(message);
  });
});

describe(stringifyEncryptedMessage.name, () => {
  it('should stringify an encrypted message', () => {
    const message = 'test';
    const key = generateEncryptionKey({ secret: mockSecret });
    const encrypted = encryptMessage(message, key);
    const stringified = stringifyEncryptedMessage(encrypted);
    expect(stringified).toBe(
      `${encrypted.authTag}${encrypted.iv}${encrypted.content}`,
    );
  });
});

describe(parseEncryptedMessage.name, () => {
  it('should parse an encrypted message', () => {
    const message = 'test';
    const key = generateEncryptionKey({ secret: mockSecret });
    const encrypted = encryptMessage(message, key);
    const stringified = stringifyEncryptedMessage(encrypted);
    const parsed = parseEncryptedMessage(stringified);
    expect(parsed).toEqual(encrypted);
  });
});
