import type { EncryptedMessage } from '@steggit/types';

export default function stringifyEncryptedMessage(
  encryptedMessage: EncryptedMessage,
) {
  return `${encryptedMessage.authTag}${encryptedMessage.iv}${encryptedMessage.content}`;
}
