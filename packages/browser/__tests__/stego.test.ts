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
    const inputPath = getInputPath('test.jpg');
    const input = fs.readFileSync(inputPath);
    await expect(encodeTextPng(input, 'test')).rejects.toThrow('Input is not a PNG image');
  });
});

