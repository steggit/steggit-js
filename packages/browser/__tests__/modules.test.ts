/* eslint-disable no-underscore-dangle */
import fs from 'node:fs';
import path, { dirname } from 'node:path';
import { fileURLToPath } from 'node:url';
import {
  encodeTextPng,
  encodeTextJpeg,
  decodeTextPng,
  decodeTextJpeg,
} from '../src/index';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const getInputPath = (name: string) => {
  return path.join(__dirname, '..', '__fixtures__', name);
};

const getInputFile = (name: string) => {
  const inputPath = getInputPath(name);
  const type = name.endsWith('.png') ? 'image/png' : 'image/jpeg';
  return new File([fs.readFileSync(inputPath)], name, { type });
};

const getInputString = (name: string) => {
  const inputPath = getInputPath(name);
  return fs.readFileSync(inputPath, 'base64');
};

describe('encode png - error handling', () => {
  it('should throw if missing arguments', async () => {
    const inputFile = getInputFile('test.png');
    const message = 'test';
    await expect(encodeTextPng(inputFile, '')).rejects.toThrow(
      'No message provided',
    );
    await expect(encodeTextPng('', message)).rejects.toThrow(
      'No input provided',
    );
    await expect(
      encodeTextPng(new File([], 'test.png'), message),
    ).rejects.toThrow('No input provided');
  });

  it('should throw if input is not a PNG image', async () => {
    const inputFile = getInputFile('test.jpg');
    await expect(encodeTextPng(inputFile, 'test')).rejects.toThrow(
      'File is not a valid png image',
    );

    const inputString = getInputString('test.jpg');
    await expect(encodeTextPng(inputString, 'test')).rejects.toThrow(
      'Input string is not a valid png image',
    );
  });

  it('should throw if message is too long for image to fit', async () => {
    const inputFile = getInputFile('tiny_test.png');
    const message = 'Hello, world!'.repeat(1000);
    await expect(encodeTextPng(inputFile, message)).rejects.toThrow(
      /Message too large for image! Available space = \d+ bits, message size = \d+ bits/,
    );
  });
});

describe('encode jpeg - error handling', () => {
  it('should throw if missing arguments', async () => {
    const inputPath = getInputPath('test.jpg');
    const input = fs.readFileSync(inputPath);
    const message = 'test';
    await expect(
      encodeTextJpeg(new File([input], 'test.jpg'), ''),
    ).rejects.toThrow('No message provided');
    await expect(encodeTextJpeg('', message)).rejects.toThrow(
      'No input provided',
    );
    await expect(
      encodeTextJpeg(new File([], 'test.jpg'), message),
    ).rejects.toThrow('No input provided');
  });

  it('should throw if input is not a JPEG image', async () => {
    const inputPngPath = getInputPath('test.png');
    const input = fs.readFileSync(inputPngPath);

    // File
    const file = new File([input], 'test.png', { type: 'image/png' });
    await expect(encodeTextJpeg(file, 'test')).rejects.toThrow(
      'File is not a valid jpeg image',
    );

    // String
    const inputString = fs.readFileSync(inputPngPath, 'base64');
    await expect(encodeTextJpeg(inputString, 'test')).rejects.toThrow(
      'Input string is not a valid jpeg image',
    );
  });

  it('should throw if message is too long for image to fit', async () => {
    const inputFile = getInputFile('tiny_test.jpg');
    const message = 'Hello, world!'.repeat(1000);
    await expect(encodeTextJpeg(inputFile, message)).rejects.toThrow(
      /Message too large for image! Available space = \d+ bits, message size = \d+ bits/,
    );
  });
});

describe('png - full process', () => {
  it('should encode and decode text in a PNG image', async () => {
    const message = 'Hello, world!';
    const inputFile = getInputFile('test.png');
    const output = await encodeTextPng(inputFile, message);
    expect(output).toBeInstanceOf(Blob);
    const outputFile = new File([output], 'test.png', { type: 'image/png' });
    const decodedMessage = await decodeTextPng(outputFile);
    expect(decodedMessage).toBe(message);
  });

  it('should encode and decode text in a PNG image with custom header', async () => {
    const inputFile = getInputFile('test.png');
    const message = 'Hello, world!';
    const header = 'abcde12345';
    const output = await encodeTextPng(inputFile, message, header);
    expect(output).toBeInstanceOf(Blob);
    const outputFile = new File([output], 'test.png', { type: 'image/png' });
    const decodedMessage = await decodeTextPng(outputFile, header);
    expect(decodedMessage).toBe(message);
  });

  it('should encode and decode text in a transparent image', async () => {
    const inputFile = getInputFile('transparent_test.png');
    const message = 'Can you see me?';
    const output = await encodeTextPng(inputFile, message);
    expect(output).toBeInstanceOf(Blob);
    const outputFile = new File([output], 'transparent_test.png', {
      type: 'image/png',
    });
    const decodedMessage = await decodeTextPng(outputFile);
    expect(decodedMessage).toBe(message);
  });

  it('should not return message if different header is used', async () => {
    const inputFile = getInputFile('test.png');
    const message = 'Hello, world!';
    const header = 'abcde12345';
    const output = await encodeTextPng(inputFile, message, header);
    expect(output).toBeInstanceOf(Blob);
    const outputFile = new File([output], 'test.png', { type: 'image/png' });
    await expect(decodeTextPng(outputFile, 'different-header')).rejects.toThrow(
      'Error: Failed to extract message from PNG image',
    );
  });
});

describe('jpeg - full process', () => {
  it('should encode and decode text in a JPEG image', async () => {
    const inputFile = getInputFile('test.jpg');
    const message = 'Hello, world!';
    const output = await encodeTextJpeg(inputFile, message);
    expect(output).toBeInstanceOf(Blob);
    const outputFile = new File([output], 'test.jpg', { type: 'image/jpeg' });
    const decodedMessage = await decodeTextJpeg(outputFile);
    expect(decodedMessage).toBe(message);
  });

  it('should encode and decode text in a JPEG image with custom header', async () => {
    const inputFile = getInputFile('test.jpg');
    const message = 'Hello, world!';
    const header = 'abcde12345';
    const output = await encodeTextJpeg(inputFile, message, header);
    expect(output).toBeInstanceOf(Blob);
    const outputFile = new File([output], 'test.jpg', { type: 'image/jpeg' });
    const decodedMessage = await decodeTextJpeg(outputFile, header);
    expect(decodedMessage).toBe(message);
  });

  it('should not return message if different header is used', async () => {
    const inputFile = getInputFile('test.jpg');
    const message = 'Hello, world!';
    const header = 'abcde12345';
    const output = await encodeTextJpeg(inputFile, message, header);
    expect(output).toBeInstanceOf(Blob);
    const outputFile = new File([output], 'test.jpg', { type: 'image/jpeg' });
    await expect(
      decodeTextJpeg(outputFile, 'different-header'),
    ).rejects.toThrow('Error: Failed to extract message from JPEG image');
  });
});
