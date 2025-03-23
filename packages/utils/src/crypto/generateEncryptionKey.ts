import { createHmac } from 'node:crypto';
import { DEFAULT_KEY_LENGTH } from './const';

export default function generateEncryptionKey(
  input: string,
  secret: string,
  length = DEFAULT_KEY_LENGTH,
  ctx = 'default',
) {
  const pseudoRandomKey = createHmac('sha256', secret)
    .update(input)
    .digest() as Buffer<ArrayBuffer>;

  let previousBlock = Buffer.alloc(0);
  let outputKeyMaterial = Buffer.alloc(0);

  for (let i = 0; outputKeyMaterial.length < length; i += 1) {
    previousBlock = createHmac('sha256', pseudoRandomKey)
      .update(
        Buffer.concat([previousBlock, Buffer.from(ctx), Buffer.from([i + 1])]),
      )
      .digest() as Buffer<ArrayBuffer>;

    outputKeyMaterial = Buffer.concat([outputKeyMaterial, previousBlock]);
  }

  return outputKeyMaterial.subarray(0, length).toString('hex');
}
