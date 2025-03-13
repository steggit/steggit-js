import { zeroAlias, oneAlias } from './invisibleChars';

export default function binaryToInvisible(byteArr: Uint8Array) {
  const bitStrings = Array.from(byteArr, (byte) =>
    byte.toString(2).padStart(8, '0'),
  );
  const bitString = bitStrings.join('');
  const invisibleString = bitString
    .replace(/0/g, zeroAlias)
    .replace(/1/g, oneAlias);

  return invisibleString;
}
