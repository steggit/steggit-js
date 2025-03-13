import { oneAlias, zeroAlias } from './invisibleChars';

export default function invisibleToBinary(invisibleString: string) {
  const binaryString = invisibleString
    .replace(new RegExp(zeroAlias, 'g'), '0')
    .replace(new RegExp(oneAlias, 'g'), '1');

  const bytes = new Uint8Array(Math.ceil(binaryString.length / 8));
  for (let i = 0; i < bytes.length; i += 1) {
    const byte = binaryString.substring(i * 8, i * 8 + 8);
    bytes[i] = parseInt(byte, 2);
  }
  return bytes;
}
