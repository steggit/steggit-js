import {
  binaryToInvisible,
  stringToBinary,
  zeroAlias,
  oneAlias,
  obfuscateString,
  deobfuscateString,
} from '../lib/string';

const helloBinary = new Uint8Array([104, 101, 108, 108, 111]);

describe(stringToBinary.name, () => {
  it('should convert a string to a binary string', () => {
    const binary = stringToBinary('hello');
    expect(binary).toEqual(helloBinary);
  });
});

describe(binaryToInvisible.name, () => {
  it('should convert a binary string to an invisible string', () => {
    const invisible = binaryToInvisible(helloBinary);
    expect(invisible.length).toEqual(40); // 5 * 8 characters
    expect(invisible).toMatch(/^[\u200B\u200C]+$/);
    expect(invisible).toContain(zeroAlias);
    expect(invisible).toContain(oneAlias);
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
