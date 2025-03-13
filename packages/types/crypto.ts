export interface EncryptedMessage {
  iv: string;
  content: string;
  authTag: string;
}
