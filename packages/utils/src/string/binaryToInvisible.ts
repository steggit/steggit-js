import { ZERO_ALIAS, ONE_ALIAS } from './const';

export default function binaryToInvisible(byteArr: Uint8Array) {
  const bitStrings = Array.from(byteArr, (byte) =>
    byte.toString(2).padStart(8, '0'),
  );
  const bitString = bitStrings.join('');
  const invisibleString = bitString
    .replace(/0/g, ZERO_ALIAS)
    .replace(/1/g, ONE_ALIAS);

  return invisibleString;
}
