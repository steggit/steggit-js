export default function zipStrings(stringOne: string, stringTwo: string) {
  if (!stringOne) {
    throw new Error('Parent string is required');
  }
  if (!stringTwo) {
    throw new Error('Child string is required');
  }

  let i = 0;
  let j = 0;
  const n = stringOne.length;
  const m = stringTwo.length;

  let result = '';

  // While there are still characters to pick from either string
  while (i < n || j < m) {
    // If we've used fewer s1-chars relative to its total, pick from stringOne
    // Equivalently, compare (i / n) vs (j / m). Smaller ratio => pick next from that string.
    //
    // Note: handle edge cases when n or m = 0 to avoid 1/0 issues.
    const ratio1 = n > 0 ? i / n : Infinity; // If n=0, we only have stringTwo
    const ratio2 = m > 0 ? j / m : Infinity; // If m=0, we only have stringOne

    if (ratio1 <= ratio2 && i < n) {
      // Take from stringOne when ratio is lower and characters remain
      result += stringOne[i];
      i += 1;
    } else if (ratio1 > ratio2 && j < m) {
      // Take from stringTwo when ratio is lower and characters remain
      result += stringTwo[j];
      j += 1;
    } else if (i < n) {
      // Fallback to stringOne if it has remaining characters
      result += stringOne[i];
      i += 1;
    } else if (j < m) {
      // Fallback to stringTwo if it has remaining characters
      result += stringTwo[j];
      j += 1;
    }
  }

  return result;
}
