export default function stripCharacters(
  string: string,
  characters: string[],
  invert = false,
) {
  const uniqueCharacters = [...new Set(characters)];
  const regex = new RegExp(
    `[${invert ? '^' : ''}${uniqueCharacters.join('')}]`,
    'g',
  );
  return string.replace(regex, '');
}
