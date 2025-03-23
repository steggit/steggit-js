import fs from 'fs';
import path from 'path';
import {
  encodeTextPng,
  encodeTextJpeg,
  decodeTextPng,
  decodeTextJpeg,
} from '../src/index';

const getInputPath = (name: string) => {
  return path.join(__dirname, '..', '__fixtures__', name);
};

describe('encode png - error handling', () => {
  it('should throw if missing arguments', async () => {
    const inputPath = getInputPath('test.png');
    const input = fs.readFileSync(inputPath);
    const message = 'test';
    await expect(encodeTextPng(input, '')).rejects.toThrow(
      'No message provided',
    );
    await expect(encodeTextPng('', message)).rejects.toThrow(
      'No input provided',
    );
    await expect(
      encodeTextPng(new File([], 'test.png'), message),
    ).rejects.toThrow('No input provided');
    await expect(encodeTextPng(Buffer.from([]), message)).rejects.toThrow(
      'No input provided',
    );
  });

  it('should throw if input is not a PNG image', async () => {
    const inputJpegPath = getInputPath('test.jpg');

    // Buffer
    const input = fs.readFileSync(inputJpegPath);
    await expect(encodeTextPng(input, 'test')).rejects.toThrow(
      'Error: Input file must be a PNG image',
    );

    // File
    const file = new File([input], 'test.jpg', { type: 'image/jpeg' });
    await expect(encodeTextPng(file, 'test')).rejects.toThrow(
      'File is not a valid png image',
    );

    // String
    const inputString = fs.readFileSync(inputJpegPath, 'base64');
    await expect(encodeTextPng(inputString, 'test')).rejects.toThrow(
      'Input string is not a valid png image',
    );
  });

  it('should throw if message is too long for image to fit', async () => {
    const inputPath = getInputPath('tiny_test.png');
    const input = fs.readFileSync(inputPath);
    const message = 'Hello, world!'.repeat(1000);
    await expect(encodeTextPng(input, message)).rejects.toThrow(
      /Message too large for image! Available space = \d+ bits, message size = \d+ bits/,
    );
  });
});

describe('encode jpeg - error handling', () => {
  it('should throw if missing arguments', async () => {
    const inputPath = getInputPath('test.jpg');
    const input = fs.readFileSync(inputPath);
    const message = 'test';
    await expect(encodeTextJpeg(input, '')).rejects.toThrow(
      'No message provided',
    );
    await expect(encodeTextJpeg('', message)).rejects.toThrow(
      'No input provided',
    );
    await expect(
      encodeTextJpeg(new File([], 'test.jpg'), message),
    ).rejects.toThrow('No input provided');
    await expect(encodeTextJpeg(Buffer.from([]), message)).rejects.toThrow(
      'No input provided',
    );
  });

  it('should throw if input is not a JPEG image', async () => {
    const inputPngPath = getInputPath('test.png');

    // Buffer
    const input = fs.readFileSync(inputPngPath);
    await expect(encodeTextJpeg(input, 'test')).rejects.toThrow(
      'Error: Input file must be a JPEG image',
    );

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
    const inputPath = getInputPath('tiny_test.jpg');
    const input = fs.readFileSync(inputPath);
    const message = 'Hello, world!'.repeat(1000);
    await expect(encodeTextJpeg(input, message)).rejects.toThrow(
      /Message too large for image! Available space = \d+ bits, message size = \d+ bits/,
    );
  });
});

describe('png - full process', () => {
  it('should encode and decode text in a PNG image', async () => {
    const inputPath = getInputPath('test.png');
    const message = 'Hello, world!';
    const input = fs.readFileSync(inputPath);
    const output = await encodeTextPng(input, message);
    expect(output).toBeInstanceOf(Blob);
    const outputFile = Buffer.from(await output.arrayBuffer());
    const decodedMessage = await decodeTextPng(outputFile);
    expect(decodedMessage).toBe(message);
  });

  it('should encode and decode text in a PNG image with custom header', async () => {
    const inputPath = getInputPath('test.png');
    const message = 'Hello, world!';
    const header = 'abcde12345';
    const input = fs.readFileSync(inputPath);
    const output = await encodeTextPng(input, message, header);
    expect(output).toBeInstanceOf(Blob);
    const outputFile = Buffer.from(await output.arrayBuffer());
    const decodedMessage = await decodeTextPng(outputFile, header);
    expect(decodedMessage).toBe(message);
  });

  it('should encode and decode text in a transparent image', async () => {
    const inputPath = getInputPath('transparent_test.png');
    const message = 'Can you see me?';
    const input = fs.readFileSync(inputPath);
    const output = await encodeTextPng(input, message);
    expect(output).toBeInstanceOf(Blob);
    const outputFile = Buffer.from(await output.arrayBuffer());
    const decodedMessage = await decodeTextPng(outputFile);
    expect(decodedMessage).toBe(message);
  });

  it('should not return message if different header is used', async () => {
    const inputPath = getInputPath('test.png');
    const message = 'Hello, world!';
    const header = 'abcde12345';
    const input = fs.readFileSync(inputPath);
    const output = await encodeTextPng(input, message, header);
    expect(output).toBeInstanceOf(Blob);
    const outputFile = Buffer.from(await output.arrayBuffer());
    await expect(decodeTextPng(outputFile, 'different-header')).rejects.toThrow(
      'No message found',
    );
  });
});

describe('jpeg - full process', () => {
  it('should encode and decode text in a JPEG image', async () => {
    const inputPath = getInputPath('test.jpg');
    const message = 'Hello, world!';
    const input = fs.readFileSync(inputPath);
    const output = await encodeTextJpeg(input, message);
    expect(output).toBeInstanceOf(Blob);
    const outputFile = Buffer.from(await output.arrayBuffer());
    const decodedMessage = await decodeTextJpeg(outputFile);
    expect(decodedMessage).toBe(message);
  });

  it('should encode and decode text in a JPEG image with custom header', async () => {
    const inputPath = getInputPath('test.jpg');
    const message = 'Hello, world!';
    const header = 'abcde12345';
    const input = fs.readFileSync(inputPath);
    const output = await encodeTextJpeg(input, message, header);
    expect(output).toBeInstanceOf(Blob);
    const outputFile = Buffer.from(await output.arrayBuffer());
    const decodedMessage = await decodeTextJpeg(outputFile, header);
    expect(decodedMessage).toBe(message);
  });

  it('should not return message if different header is used', async () => {
    const inputPath = getInputPath('test.jpg');
    const message = 'Hello, world!';
    const header = 'abcde12345';
    const input = fs.readFileSync(inputPath);
    const output = await encodeTextJpeg(input, message, header);
    expect(output).toBeInstanceOf(Blob);
    const outputFile = Buffer.from(await output.arrayBuffer());
    await expect(
      decodeTextJpeg(outputFile, 'different-header'),
    ).rejects.toThrow('No message found');
  });
});
