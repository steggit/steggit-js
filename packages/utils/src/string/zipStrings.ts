export default function zipStrings(stringOne: string, stringTwo: string) {
  if (!stringOne) {
    throw new Error('Parent string is required');
  }
  if (!stringTwo) {
    throw new Error('Child string is required');
  }

  // Calculate the greatest common divisor to help find optimal chunk sizes
  const gcd = (a: number, b: number): number => {
    return b === 0 ? a : gcd(b, a % b);
  };

  const len1 = stringOne.length;
  const len2 = stringTwo.length;

  // Find the GCD of the two lengths to determine optimal chunk sizes
  const divisor = gcd(len1, len2);

  // Calculate chunk sizes based on the GCD to ensure even distribution
  const chunkSize1 = Math.max(1, Math.floor(len1 / (len1 / divisor)));
  const chunkSize2 = Math.max(1, Math.floor(len2 / (len2 / divisor)));

  let result = '';
  let i = 0;
  let j = 0;

  while (i < len1 || j < len2) {
    if (i < len1) {
      result += stringOne.substring(i, i + chunkSize1);
      i += chunkSize1;
    }

    if (j < len2) {
      result += stringTwo.substring(j, j + chunkSize2);
      j += chunkSize2;
    }
  }

  return result;
}
