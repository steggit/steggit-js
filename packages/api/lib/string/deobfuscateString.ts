export default function deobfuscateString(
  input: string,
  obfuscatorChar: string,
) {
  // find first instance of markerAlias
  const markerIndex = input.indexOf(obfuscatorChar);
  if (markerIndex === -1) {
    throw new Error('Start marker not found');
  }

  // find last instance of markerAlias
  const lastMarkerIndex = input.lastIndexOf(obfuscatorChar);
  if (lastMarkerIndex === -1) {
    throw new Error('End marker not found');
  }

  // remove all ZWJ and ZWSP outside of marker strings
  const sliced = input.slice(markerIndex, lastMarkerIndex);

  // Remove all markerAlias instances
  const result = sliced.replaceAll(obfuscatorChar, '');

  return result;
}
