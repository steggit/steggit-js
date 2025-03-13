import type { EncryptedMessage } from '../../types/crypto';

export default function stringifyEncryptedMessage(
  encryptedMessage: EncryptedMessage,
) {
  return `${encryptedMessage.authTag}${encryptedMessage.iv}${encryptedMessage.content}`;
}
