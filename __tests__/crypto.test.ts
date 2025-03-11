import {
  decryptMessage,
  encryptMessage,
  generateEncryptionKey,
  generateSharedUserSecret,
  generateUserSecret,
  getGlobalSecret,
} from '../lib/crypto';
import type { User } from '../types/user';

const mockUserOne: User = {
  id: '1',
  name: 'John Doe',
  secret: '0987654321',
};

const mockUserTwo: User = {
  id: '2',
  name: 'Jane Doe',
  secret: '1234567890',
};

describe(getGlobalSecret.name, () => {
  it('should return the global secret', () => {
    const secret = getGlobalSecret();
    expect(secret).toBe('abcde12345');
  });
});

describe(generateUserSecret.name, () => {
  it('should return a user secret', () => {
    const secret = generateUserSecret();
    expect(secret).toBeDefined();
    expect(secret.length).toBe(64);
  });

  it('should return a different secret each time', () => {
    const secret1 = generateUserSecret();
    const secret2 = generateUserSecret();
    expect(secret1).not.toBe(secret2);
  });
});

describe(generateSharedUserSecret.name, () => {
  it('should return a shared user secret', () => {
    const secret = generateSharedUserSecret([mockUserTwo, mockUserOne]);
    expect(secret).toBeDefined();
    expect(secret).toBe(mockUserOne.secret + mockUserTwo.secret);
  });
});

describe(generateEncryptionKey.name, () => {
  it('should return an encryption key', () => {
    const key = generateEncryptionKey('test', 32);
    expect(key.length).toBe(64);
  });

  it('should return the same key with the same input', () => {
    const key1 = generateEncryptionKey('test', 32);
    const key2 = generateEncryptionKey('test', 32);
    expect(key1).toBe(key2);
  });

  it('should return a different key with different input', () => {
    const key1 = generateEncryptionKey('test', 32);
    const key2 = generateEncryptionKey('test2', 32);
    expect(key1).not.toBe(key2);
  });
});

describe(encryptMessage.name, () => {
  it('should throw if key is too short', () => {
    const message = 'test';
    const key = generateEncryptionKey('test', 16);
    expect(() => encryptMessage(message, key)).toThrow('Invalid key length');
  });

  it('should encrypt a message', () => {
    const message = 'test';
    const key = generateEncryptionKey('test', 32);
    const encrypted = encryptMessage(message, key);
    expect(encrypted).toBeDefined();
    expect(encrypted.iv.length).toBe(24);
    expect(encrypted.content.length).toBe(8);
    expect(encrypted.authTag.length).toBe(32);
    expect(encrypted.content).not.toBe(message);
  });

  it('the same message should be encrypted differently each time', () => {
    const message = 'test';
    const key = generateEncryptionKey('test', 32);
    const encrypted1 = encryptMessage(message, key);
    const encrypted2 = encryptMessage(message, key);
    expect(encrypted1).not.toBe(encrypted2);
  });
});

describe(decryptMessage.name, () => {
  it('should throw an error if key is incorrect', () => {
    const message = 'test';
    const key = generateEncryptionKey('test', 32);
    const encrypted = encryptMessage(message, key);
    const wrongKey = generateEncryptionKey('wrong', 32);
    expect(() => decryptMessage(encrypted, wrongKey)).toThrow(
      'Unsupported state or unable to authenticate data',
    );
  });

  it('should decrypt a message', () => {
    const message = 'test';
    const key = generateEncryptionKey('test', 32);
    const encrypted = encryptMessage(message, key);
    const decrypted = decryptMessage(encrypted, key);
    expect(decrypted).toBe(message);
    expect(decrypted).not.toBe(encrypted.content);
  });

  it('the same message should be decrypted the same each time', () => {
    const message = 'test';
    const key = generateEncryptionKey('test', 32);
    const encrypted1 = encryptMessage(message, key);
    const encrypted2 = encryptMessage(message, key);
    const decrypted1 = decryptMessage(encrypted1, key);
    const decrypted2 = decryptMessage(encrypted2, key);
    expect(decrypted1).toBe(message);
    expect(decrypted2).toBe(message);
  });

  it('should decrypt message using shared user secret', () => {
    const message = 'test';
    const sharedSecret = generateSharedUserSecret([mockUserOne, mockUserTwo]);
    const sharedSecret2 = generateSharedUserSecret([mockUserTwo, mockUserOne]);
    const key1 = generateEncryptionKey(sharedSecret, 32);
    const key2 = generateEncryptionKey(sharedSecret2, 32);
    const encrypted = encryptMessage(message, key1);
    const decrypted = decryptMessage(encrypted, key2);
    expect(decrypted).toBe(message);
  });
});
