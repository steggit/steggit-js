import { DEFAULT_IV_LENGTH, DEFAULT_KEY_LENGTH } from '.';
import type { EncryptedMessage } from '../../types/crypto';

export default function parseEncryptedMessage(
  stringifiedMessage: string,
): EncryptedMessage {
  const authTag = stringifiedMessage.slice(0, DEFAULT_KEY_LENGTH);
  const iv = stringifiedMessage.slice(
    DEFAULT_KEY_LENGTH,
    DEFAULT_KEY_LENGTH + DEFAULT_IV_LENGTH,
  );
  const content = stringifiedMessage.slice(
    DEFAULT_KEY_LENGTH + DEFAULT_IV_LENGTH,
  );
  return { authTag, iv, content };
}
