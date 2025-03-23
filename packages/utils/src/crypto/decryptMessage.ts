import { createDecipheriv } from 'node:crypto';
import type { EncryptedMessage } from './types';

export default function decryptMessage(
  encrypted: EncryptedMessage,
  key: string,
) {
  const decipher = createDecipheriv(
    'aes-256-gcm',
    Buffer.from(key, 'hex'),
    encrypted.iv,
  );
  decipher.setAuthTag(Buffer.from(encrypted.authTag, 'hex'));

  const decryptedMessage = Buffer.concat([
    decipher.update(Buffer.from(encrypted.content, 'hex')),
    decipher.final(),
  ]);

  return decryptedMessage.toString();
}
