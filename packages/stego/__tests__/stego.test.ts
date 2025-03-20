import fs from 'fs';
import path from 'path';
import { encodeTextPng, encodeTextJpeg } from '../index';

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

describe(encodeTextPng.name, () => {
  it('should throw if missing arguments', async () => {
    const inputPath = getInputPath('test.png');
    const message = 'test';
    await expect(encodeTextPng(inputPath, pngOutputPath, '')).rejects.toThrow('Message is required');
    await expect(encodeTextPng('', pngOutputPath, message)).rejects.toThrow('Input file is required');
    await expect(encodeTextPng(inputPath, '', message)).rejects.toThrow('Output file is required');
  });
  
  it('should throw if input file not found', async () => {
    const inputPath = getInputPath('not-found.png');
    const message = 'Hello, world!';
    await expect(encodeTextPng(inputPath, pngOutputPath, message)).rejects.toThrow('Input file does not exist');
  });

  it('should throw if input file is not a PNG image', async () => {
    const inputPath = getInputPath('test.jpg');
    const message = 'Hello, world!';
    await expect(encodeTextPng(inputPath, pngOutputPath, message)).rejects.toThrow('Input file must be a PNG image');
  });

  it('should encode and decode text in a PNG image', async () => {
    const inputPath = getInputPath('test.png');
    const message = 'Hello, world!';
    await expect(encodeTextPng(inputPath, pngOutputPath, message)).resolves.toBeUndefined();
  });
});

describe(encodeTextJpeg.name, () => {
  it('should throw if missing arguments', async () => {
    const inputPath = getInputPath('test.jpg');
    const message = 'test';
    await expect(encodeTextJpeg(inputPath, jpegOutputPath, '')).rejects.toThrow('Message is required');
    await expect(encodeTextJpeg('', jpegOutputPath, message)).rejects.toThrow('Input file is required');
    await expect(encodeTextJpeg(inputPath, '', message)).rejects.toThrow('Output file is required');
  });
  
  it('should throw if input file not found', async () => {
    const inputPath = getInputPath('not-found.jpg');
    const message = 'Hello, world!';
    await expect(encodeTextJpeg(inputPath, jpegOutputPath, message)).rejects.toThrow('Input file does not exist');
  });

  it('should throw if input file is not a JPEG image', async () => {
    const inputPath = getInputPath('test.png');
    const message = 'Hello, world!';
    await expect(encodeTextJpeg(inputPath, jpegOutputPath, message)).rejects.toThrow('Input file must be a JPEG image');
  });

  it('should encode and decode text in a JPEG image', async () => {
    const inputPath = getInputPath('test.jpg');
    const message = 'Hello, world!';
    await expect(encodeTextJpeg(inputPath, jpegOutputPath, message)).resolves.toBeUndefined();
  });
});