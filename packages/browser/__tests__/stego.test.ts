import fs from 'fs';
import path from 'path';
import { encodeTextPng, encodeTextJpeg, decodeTextPng, decodeTextJpeg } from '../src/index';

const getInputPath = (name: string) => {
  return path.join(__dirname, '..', '__fixtures__', name);
};

const jpegOutputPath = getInputPath('output.jpg');
const pngOutputPath = getInputPath('output.png');

beforeAll(() => {
  if (fs.existsSync(jpegOutputPath)) {
    fs.unlinkSync(jpegOutputPath);
  }
  if (fs.existsSync(pngOutputPath)) {
    fs.unlinkSync(pngOutputPath);
  }
});

afterAll(() => {
  if (fs.existsSync(jpegOutputPath)) {
    fs.unlinkSync(jpegOutputPath);
  }
  if (fs.existsSync(pngOutputPath)) {
    fs.unlinkSync(pngOutputPath);
  }
});

describe('encode png - error handling', () => {
  it('should throw if missing arguments', async () => {
    const inputPath = getInputPath('test.png');
    const input = fs.readFileSync(inputPath);
    const message = 'test';
    await expect(encodeTextPng(input, '')).rejects.toThrow('No message provided');
    await expect(encodeTextPng('', message)).rejects.toThrow('No input provided');
  });

  it('should throw if input is not a PNG image', async () => {
    const inputJpegPath = getInputPath('test.jpg');

    // Buffer
    const input = fs.readFileSync(inputJpegPath);
    await expect(encodeTextPng(input, 'test')).rejects.toThrow('Error: Input file must be a PNG image');

    // File
    const file = new File([input], 'test.jpg', { type: 'image/jpeg' });
    await expect(encodeTextPng(file, 'test')).rejects.toThrow('File is not a valid png image');

    // String
    const inputString = fs.readFileSync(inputJpegPath, 'base64');
    await expect(encodeTextPng(inputString, 'test')).rejects.toThrow('Input string is not a valid png image');
  });

  it('should throw if message is too long for image to fit', async () => {
    const inputPath = getInputPath('tiny_test.png');
    const input = fs.readFileSync(inputPath);
    const message = 'Hello, world!'.repeat(1000);
    await expect(encodeTextPng(input, message)).rejects.toThrow(/Message too large for image! Available space = \d+ bits, message size = \d+ bits/);
  });
});

describe('encode jpeg - error handling', () => {
  it('should throw if missing arguments', async () => {
    const inputPath = getInputPath('test.jpg');
    const input = fs.readFileSync(inputPath);
    const message = 'test';
    await expect(encodeTextJpeg(input, '')).rejects.toThrow('No message provided');
    await expect(encodeTextJpeg('', message)).rejects.toThrow('No input provided');
  });

  it('should throw if input is not a JPEG image', async () => {
    const inputPngPath = getInputPath('test.png');

    // Buffer
    const input = fs.readFileSync(inputPngPath);
    await expect(encodeTextJpeg(input, 'test')).rejects.toThrow('Error: Input file must be a JPEG image');

    // File
    const file = new File([input], 'test.png', { type: 'image/png' });
    await expect(encodeTextJpeg(file, 'test')).rejects.toThrow('File is not a valid jpeg image');

    // String
    const inputString = fs.readFileSync(inputPngPath, 'base64');
    await expect(encodeTextJpeg(inputString, 'test')).rejects.toThrow('Input string is not a valid jpeg image');
  });

  it('should throw if message is too long for image to fit', async () => {
    const inputPath = getInputPath('tiny_test.jpg');
    const input = fs.readFileSync(inputPath);
    const message = 'Hello, world!'.repeat(1000);
    await expect(encodeTextJpeg(input, message)).rejects.toThrow(/Message too large for image! Available space = \d+ bits, message size = \d+ bits/);
  });
});