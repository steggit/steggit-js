export default function zipStrings(
  stringOne: string,
  stringTwo: string,
  maxChunkSize: number,
) {
  if (maxChunkSize <= 0) {
    throw new Error('maxChunkSize must be greater than 0');
  }

  let result = '';
  let indexOne = 0;
  let indexTwo = 0;

  while (indexOne < stringOne.length || indexTwo < stringTwo.length) {
    // Determine if we should take from stringOne or stringTwo next
    // If one string is exhausted, we'll take from the other
    const takeFromOne =
      indexTwo >= stringTwo.length ||
      (indexOne < stringOne.length && Math.random() < 0.5);

    if (takeFromOne) {
      // Determine random chunk size between 1 and maxChunkSize
      const remainingChars = stringOne.length - indexOne;
      const chunkSize = Math.min(
        remainingChars,
        1 + Math.floor(Math.random() * maxChunkSize),
      );

      // Add chunk from stringOne to result
      result += stringOne.substring(indexOne, indexOne + chunkSize);
      indexOne += chunkSize;
    } else {
      // Determine random chunk size between 1 and maxChunkSize
      const remainingChars = stringTwo.length - indexTwo;
      const chunkSize = Math.min(
        remainingChars,
        1 + Math.floor(Math.random() * maxChunkSize),
      );

      // Add chunk from stringTwo to result
      result += stringTwo.substring(indexTwo, indexTwo + chunkSize);
      indexTwo += chunkSize;
    }
  }

  return result;
}
