import path from 'path';
import { encodeTextPng, decodeTextPng } from '../index';

const getInputPath = (name: string) => {
  return path.join(__dirname, '..', '..', '__fixtures__', name);
};

describe('stego', () => {
  it('should throw if message is blank', async () => {
    const inputPath = getInputPath('test.png');
    const outputPath = getInputPath('output.png');
    const message = '';
    expect(() => encodeTextPng(inputPath, outputPath, message)).toThrow('Message is required');
  });
  
  it('should throw if input file not found', async () => {
    const inputPath = getInputPath('not-found.png');
    const outputPath = getInputPath('output.png');
    const message = 'Hello, world!';
    expect(() => encodeTextPng(inputPath, outputPath, message)).toThrow('Input file does not exist');
  });

  it('should throw if input file is not a PNG image', async () => {
    const inputPath = getInputPath('test.jpg');
    const outputPath = getInputPath('output.png');
    const message = 'Hello, world!';
    expect(() => encodeTextPng(inputPath, outputPath, message)).toThrow('Input file must be a PNG image');
  });
  

  it('should encode and decode text in a PNG image', async () => {
    const inputPath = getInputPath('test.png');
    const outputPath = getInputPath('output.png');
    const message = 'Hello, world!';
    await encodeTextPng(inputPath, outputPath, message);
    const decodedMessage = await decodeTextPng(outputPath);
    expect(decodedMessage).toBe(message);
  });
});