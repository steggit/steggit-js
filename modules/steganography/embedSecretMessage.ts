import type { User } from '../../types/user';
import {
  binaryToInvisible,
  markerAlias,
  obfuscateString,
  oneAlias,
  stringToBinary,
  zeroAlias,
  zipStrings,
} from '../../lib/string';
import {
  generateSharedUserSecret,
  encryptMessage,
  generateEncryptionKey,
  stringifyEncryptedMessage,
} from '../../lib/crypto';

interface EmbedMessageInput {
  users: User[];
  privateMessage: string;
  publicMessage: string;
}

export default function embedSecretMessage({
  users,
  privateMessage,
  publicMessage,
}: EmbedMessageInput) {
  const sharedUserSecret = generateSharedUserSecret(users);
  const key = generateEncryptionKey(sharedUserSecret);
  const encryptedMessage = encryptMessage(privateMessage, key);
  const stringifiedEncryptedMessage =
    stringifyEncryptedMessage(encryptedMessage);
  const binaryEncryptedPrivateMessage = stringToBinary(
    stringifiedEncryptedMessage,
  );
  const invisibleEncryptedPrivateMessage = binaryToInvisible(
    binaryEncryptedPrivateMessage,
  );
  const obfuscatedEncryptedPrivateMessage = obfuscateString(
    invisibleEncryptedPrivateMessage,
    markerAlias,
    [oneAlias, zeroAlias],
    0.2,
  );
  const zippedMessage = zipStrings(
    obfuscatedEncryptedPrivateMessage,
    publicMessage,
    3,
  );
  return zippedMessage;
}
