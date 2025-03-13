import { createCipheriv, randomBytes } from 'node:crypto';
import type { EncryptedMessage } from '@steggit/types';
import { DEFAULT_IV_LENGTH } from '.';

export default function encryptMessage(
  message: string,
  key: string,
): EncryptedMessage {
  const iv = randomBytes(DEFAULT_IV_LENGTH / 2).toString('hex');

  const cipher = createCipheriv('aes-256-gcm', Buffer.from(key, 'hex'), iv);
  const content = Buffer.concat([
    cipher.update(message),
    cipher.final(),
  ]).toString('hex');
  const authTag = cipher.getAuthTag().toString('hex');

  return {
    iv,
    content,
    authTag,
  };
}
