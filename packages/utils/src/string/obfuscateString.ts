import crypto from 'node:crypto';

export default function obfuscateString(
  input: string,
  obfuscatorChar: string,
  tailChars: string[],
  obfuscationPercentage: number,
) {
  if (obfuscationPercentage < 0 || obfuscationPercentage > 1) {
    throw new Error('Obfuscation percentage must be between 0 and 1');
  }
  const endedString = `${obfuscatorChar}${input}${obfuscatorChar}`;
  // Determine how many markers to insert
  const numMarkers = Math.max(
    1,
    Math.floor(endedString.length * obfuscationPercentage),
  );

  // Generate random positions to insert markers
  const positions = new Set<number>();
  while (positions.size < numMarkers) {
    const randomBytes = crypto.randomBytes(2);
    const position = randomBytes.readUInt16BE(0) % endedString.length;
    positions.add(position);
  }
  // Convert to array and sort positions
  const sortedPositions = Array.from(positions).sort((a, b) => a - b);

  // Insert markers at random positions
  let result = endedString;
  for (let i = sortedPositions.length - 1; i >= 0; i -= 1) {
    const pos = sortedPositions[i] + obfuscatorChar.length;
    result = result.slice(0, pos) + obfuscatorChar + result.slice(pos);
  }

  // Add random invisible characters at the beginning and end (20% of total length)
  const additionalCharsCount = Math.max(
    1,
    Math.floor(endedString.length * obfuscationPercentage),
  );

  // Determine how many chars to add at the beginning vs end
  const beginningCharsCount = Math.floor(Math.random() * additionalCharsCount);
  const endCharsCount = additionalCharsCount - beginningCharsCount;

  let beginningPadding = '';
  for (let i = 0; i < beginningCharsCount; i += 1) {
    const randomChar = tailChars[Math.floor(Math.random() * tailChars.length)];
    beginningPadding += randomChar;
  }

  let endPadding = '';
  for (let i = 0; i < endCharsCount; i += 1) {
    const randomChar = tailChars[Math.floor(Math.random() * tailChars.length)];
    endPadding += randomChar;
  }

  // Add the padding to the result
  result = beginningPadding + result + endPadding;

  return result;
}
