import { createHmac } from 'node:crypto';
import getGlobalSecret from './getGlobalSecret';

export default function generateEncryptionKey(
  input: string,
  length: number,
  ctx = 'default',
) {
  const pseudoRandomKey = createHmac('sha256', getGlobalSecret())
    .update(input)
    .digest();

  let previousBlock = Buffer.alloc(0);
  let outputKeyMaterial = Buffer.alloc(0);

  for (let i = 0; outputKeyMaterial.length < length; i += 1) {
    previousBlock = createHmac('sha256', pseudoRandomKey)
      .update(
        Buffer.concat([previousBlock, Buffer.from(ctx), Buffer.from([i + 1])]),
      )
      .digest();

    outputKeyMaterial = Buffer.concat([outputKeyMaterial, previousBlock]);
  }

  return outputKeyMaterial.subarray(0, length).toString('hex');
}
