import path from 'path';
import { encodeTextPng, decodeTextPng } from '../index';

const getInputPath = (name: string) => {
  return path.join(__dirname, '..', '__fixtures__', name);
};

describe('stego', () => {
  it('should throw if missing arguments', async () => {
    const inputPath = getInputPath('test.png');
    const outputPath = getInputPath('output.png');
    const message = 'test';
    expect(encodeTextPng(inputPath, outputPath, '')).rejects.toThrow('Message is required');
    expect(encodeTextPng('', outputPath, message)).rejects.toThrow('Input file is required');
    expect(encodeTextPng(inputPath, '', message)).rejects.toThrow('Output file is required');
  });
  
  it('should throw if input file not found', async () => {
    const inputPath = getInputPath('not-found.png');
    const outputPath = getInputPath('output.png');
    const message = 'Hello, world!';
    expect(encodeTextPng(inputPath, outputPath, message)).rejects.toThrow('Input file does not exist');
  });

  it('should throw if input file is not a PNG image', async () => {
    const inputPath = getInputPath('test.jpg');
    const outputPath = getInputPath('output.png');
    const message = 'Hello, world!';
    expect(encodeTextPng(inputPath, outputPath, message)).rejects.toThrow('Input file must be a PNG image');
  });

  it('should encode and decode text in a PNG image', async () => {
    const inputPath = getInputPath('test.png');
    const outputPath = getInputPath('output.png');
    const message = 'Hello, world!';
    expect(encodeTextPng(inputPath, outputPath, message)).resolves.toBeUndefined();
  });
});