import type { User } from '@steggit/types';
import {
  decryptMessage,
  generateEncryptionKey,
  generateSharedUserSecret,
  parseEncryptedMessage,
} from '../../lib/crypto';
import {
  stripCharacters,
  markerAlias,
  oneAlias,
  zeroAlias,
  deobfuscateString,
  invisibleToBinary,
  binaryToString,
} from '../../lib/string';

interface ExtractSecretMessageInput {
  message: string;
  users: User[];
}

export default function extractSecretMessage({
  message,
  users,
}: ExtractSecretMessageInput) {
  const encryptedInvisibleMessage = stripCharacters(
    message,
    [markerAlias, oneAlias, zeroAlias],
    true,
  );
  const deobfuscatedInvisibleMessage = deobfuscateString(
    encryptedInvisibleMessage,
    markerAlias,
  );
  const binaryEncryptedMessage = invisibleToBinary(
    deobfuscatedInvisibleMessage,
  );
  const encryptedMessage = binaryToString(binaryEncryptedMessage);
  const parsedEncryptedMessage = parseEncryptedMessage(encryptedMessage);
  const secret = generateSharedUserSecret(users);
  const key = generateEncryptionKey(secret);
  const decryptedMessage = decryptMessage(parsedEncryptedMessage, key);
  return decryptedMessage;
}
