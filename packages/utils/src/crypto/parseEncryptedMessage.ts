import type { EncryptedMessage } from './types';
import { DEFAULT_IV_LENGTH, DEFAULT_KEY_LENGTH } from './const';

export default function parseEncryptedMessage(
  stringifiedMessage: string,
  keyLength = DEFAULT_KEY_LENGTH,
  ivLength = DEFAULT_IV_LENGTH,
): EncryptedMessage {
  const authTag = stringifiedMessage.slice(0, keyLength);
  const iv = stringifiedMessage.slice(keyLength, keyLength + ivLength);
  const content = stringifiedMessage.slice(keyLength + ivLength);
  return { authTag, iv, content };
}
