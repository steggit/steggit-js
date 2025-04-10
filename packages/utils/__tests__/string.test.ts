import {
  binaryToInvisible,
  stringToBinary,
  ZERO_ALIAS,
  ONE_ALIAS,
  obfuscateString,
  deobfuscateString,
  zipStrings,
  stripCharacters,
  invisibleToBinary,
  binaryToString,
} from '../src/string/index';

const helloBinary = new Uint8Array([104, 101, 108, 108, 111]);

describe(stringToBinary.name, () => {
  it('should convert a string to a binary string', () => {
    const binary = stringToBinary('hello');
    expect(binary).toEqual(helloBinary);
  });
});

describe(binaryToString.name, () => {
  it('should convert a binary string to a string', () => {
    const string = binaryToString(helloBinary);
    expect(string).toEqual('hello');
  });
});

describe(binaryToInvisible.name, () => {
  it('should convert a binary string to an invisible string', () => {
    const invisible = binaryToInvisible(helloBinary);
    expect(invisible.length).toEqual(40); // 5 * 8 characters
    expect(invisible).toMatch(/^[\u200B\u200C]+$/);
    expect(invisible).toContain(ZERO_ALIAS);
    expect(invisible).toContain(ONE_ALIAS);
  });
});

describe(invisibleToBinary.name, () => {
  it('should convert an invisible string to a binary string', () => {
    const invisible = binaryToInvisible(helloBinary);
    expect(invisible).toContain(ZERO_ALIAS);
    expect(invisible).toContain(ONE_ALIAS);
    const binary = invisibleToBinary(invisible);
    expect(binary).toEqual(helloBinary);
  });
});

describe(obfuscateString.name, () => {
  it('should throw an error if the obfuscation percentage is not between 0 and 1', () => {
    expect(() =>
      obfuscateString('visibletest', 'x', ['y', 'z'], 1.1),
    ).toThrow();
  });
  it('should handle empty tail characters', () => {
    const obfuscated = obfuscateString('visibletest', 'x', [], 0.2);
    expect(obfuscated).not.toEqual('visibletest');
    expect(obfuscated.length).toBeGreaterThan('visibletest'.length);
    expect(obfuscated).toContain('x');
  });
  it('should obfuscate a string', () => {
    const obfuscated = obfuscateString('visibletest', 'x', ['y', 'z'], 0.3);
    expect(obfuscated).not.toEqual('visibletest');
    expect(obfuscated.length).toBeGreaterThan('visibletest'.length);
    expect(obfuscated).toContain('x');
  });
});

describe(deobfuscateString.name, () => {
  it('should throw an error if the start marker is not found', () => {
    expect(() => deobfuscateString('visibletest', 'x')).toThrow();
  });
  it('should throw an error if the end marker is not found', () => {
    expect(() => deobfuscateString('visibletest', 'x')).toThrow();
  });
  it('should deobfuscate a string', () => {
    const obfuscated = obfuscateString('visibletest', 'x', ['y', 'z'], 0.2);
    const deobfuscated = deobfuscateString(obfuscated, 'x');
    expect(deobfuscated).toEqual('visibletest');
  });
});

describe(zipStrings.name, () => {
  it('should throw an error if the parent string is missing', () => {
    expect(() => zipStrings('', 'child')).toThrow();
  });
  it('should throw an error if the child string is missing', () => {
    expect(() => zipStrings('parent', '')).toThrow();
  });
  it('should zip two strings', () => {
    const zipped = zipStrings('parent', 'child');
    expect(zipped).toEqual('pcahrielndt');
  });
});

describe(stripCharacters.name, () => {
  it('should strip characters from a string', () => {
    const stripped = stripCharacters('hello', ['l']);
    expect(stripped).toEqual('heo');
  });

  it('should strip other characters from a string', () => {
    const stripped = stripCharacters('hello', ['o', 'e'], true);
    expect(stripped).toEqual('eo');
  });
});
