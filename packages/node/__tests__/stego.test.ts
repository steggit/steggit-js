import fs from 'fs';
import path from 'path';
import {
  encodeTextPng,
  encodeTextJpeg,
  decodeTextPng,
  decodeTextJpeg,
} from '../index';

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
    const message = 'test';
    await expect(encodeTextPng(inputPath, pngOutputPath, '')).rejects.toThrow(
      'Message is required',
    );
    await expect(encodeTextPng('', pngOutputPath, message)).rejects.toThrow(
      'Input file is required',
    );
    await expect(encodeTextPng(inputPath, '', message)).rejects.toThrow(
      'Output file is required',
    );
  });

  it('should throw if input file not found', async () => {
    const inputPath = getInputPath('not-found.png');
    const message = 'Hello, world!';
    await expect(
      encodeTextPng(inputPath, pngOutputPath, message),
    ).rejects.toThrow('Input file does not exist');
  });

  it('should throw if input file is not a PNG image', async () => {
    const inputPath = getInputPath('test.jpg');
    const message = 'Hello, world!';
    await expect(
      encodeTextPng(inputPath, pngOutputPath, message),
    ).rejects.toThrow('Input file must be a PNG image');
  });

  it('should throw if message is too long for image to fit', async () => {
    const inputPath = getInputPath('tiny_test.png');
    const message = 'Hello, world!'.repeat(1000);
    await expect(
      encodeTextPng(inputPath, pngOutputPath, message),
    ).rejects.toThrow(
      /Message too large for image! Available space = \d+ bits, message size = \d+ bits/,
    );
  });
});

describe('encode jpeg - error handling', () => {
  it('should throw if missing arguments', async () => {
    const inputPath = getInputPath('test.jpg');
    const message = 'test';
    await expect(encodeTextJpeg(inputPath, jpegOutputPath, '')).rejects.toThrow(
      'Message is required',
    );
    await expect(encodeTextJpeg('', jpegOutputPath, message)).rejects.toThrow(
      'Input file is required',
    );
    await expect(encodeTextJpeg(inputPath, '', message)).rejects.toThrow(
      'Output file is required',
    );
  });

  it('should throw if input file not found', async () => {
    const inputPath = getInputPath('not-found.jpg');
    const message = 'Hello, world!';
    await expect(
      encodeTextJpeg(inputPath, jpegOutputPath, message),
    ).rejects.toThrow('Input file does not exist');
  });

  it('should throw if input file is not a JPEG image', async () => {
    const inputPath = getInputPath('test.png');
    const message = 'Hello, world!';
    await expect(
      encodeTextJpeg(inputPath, jpegOutputPath, message),
    ).rejects.toThrow('Input file must be a JPEG image');
  });

  it('should throw if message is too long for image to fit', async () => {
    const inputPath = getInputPath('tiny_test.jpg');
    const message = 'Hello, world!'.repeat(1000);
    await expect(
      encodeTextJpeg(inputPath, jpegOutputPath, message),
    ).rejects.toThrow(
      /Message too large for image! Available space = \d+ bits, message size = \d+ bits/,
    );
  });
});

describe('decode png - error handling', () => {
  it('should throw if missing arguments', async () => {
    await expect(decodeTextPng('')).rejects.toThrow('Input file is required');
  });

  it('should throw if input file not found', async () => {
    await expect(decodeTextPng('not-found.png')).rejects.toThrow(
      'Input file does not exist',
    );
  });

  it('should throw if input file is not a PNG image', async () => {
    await expect(decodeTextPng(getInputPath('test.jpg'))).rejects.toThrow(
      'Input file must be a PNG image',
    );
  });

  it('should throw if no message found', async () => {
    await expect(decodeTextPng(getInputPath('test.png'))).rejects.toThrow(
      'Failed to decode message',
    );
  });
});

describe('decode jpeg - error handling', () => {
  it('should throw if missing arguments', async () => {
    await expect(decodeTextJpeg('')).rejects.toThrow('Input file is required');
  });

  it('should throw if input file not found', async () => {
    await expect(decodeTextJpeg('not-found.jpg')).rejects.toThrow(
      'Input file does not exist',
    );
  });

  it('should throw if input file is not a JPEG image', async () => {
    await expect(decodeTextJpeg(getInputPath('test.png'))).rejects.toThrow(
      'Input file must be a JPEG image',
    );
  });

  it('should throw if no message found', async () => {
    await expect(decodeTextJpeg(getInputPath('test.jpg'))).rejects.toThrow(
      'Failed to decode message',
    );
  });
});

describe('png - full process', () => {
  it('should encode and decode text in a PNG image', async () => {
    const inputPath = getInputPath('test.png');
    const message = 'Hello, world!';
    await expect(
      encodeTextPng(inputPath, pngOutputPath, message),
    ).resolves.toBeUndefined();
    const decodedMessage = await decodeTextPng(pngOutputPath);
    expect(decodedMessage).toBe(message);
  });

  it('should encode and decode text in a PNG image with custom header', async () => {
    const inputPath = getInputPath('test.png');
    const message = 'Hello, world!';
    const header = 'abcde12345';
    await expect(
      encodeTextPng(inputPath, pngOutputPath, message, header),
    ).resolves.toBeUndefined();
    const decodedMessage = await decodeTextPng(pngOutputPath, header);
    expect(decodedMessage).toBe(message);
  });

  it('should encode and decode text in a transparent image', async () => {
    const inputPath = getInputPath('transparent_test.png');
    const message = 'Can you see me?';
    await expect(
      encodeTextPng(inputPath, pngOutputPath, message),
    ).resolves.toBeUndefined();
    const decodedMessage = await decodeTextPng(pngOutputPath);
    expect(decodedMessage).toBe(message);
  });

  it('should not return message if different header is used', async () => {
    const inputPath = getInputPath('test.png');
    const message = 'Hello, world!';
    const header = 'abcde12345';
    await expect(
      encodeTextPng(inputPath, pngOutputPath, message, header),
    ).resolves.toBeUndefined();
    await expect(
      decodeTextPng(pngOutputPath, 'different-header'),
    ).rejects.toThrow('Failed to decode message');
  });
});

describe('jpeg - full process', () => {
  it('should encode and decode text in a JPEG image', async () => {
    const inputPath = getInputPath('test.jpg');
    const message = 'Hello, world!';
    await expect(
      encodeTextJpeg(inputPath, jpegOutputPath, message),
    ).resolves.toBeUndefined();
    const decodedMessage = await decodeTextJpeg(jpegOutputPath);
    expect(decodedMessage).toBe(message);
  });

  it('should encode and decode text in a JPEG image with custom header', async () => {
    const inputPath = getInputPath('test.jpg');
    const message = 'Hello, world!';
    const header = 'abcde12345';
    await expect(
      encodeTextJpeg(inputPath, jpegOutputPath, message, header),
    ).resolves.toBeUndefined();
    const decodedMessage = await decodeTextJpeg(jpegOutputPath, header);
    expect(decodedMessage).toBe(message);
  });

  it('should not return message if different header is used', async () => {
    const inputPath = getInputPath('test.jpg');
    const message = 'Hello, world!';
    const header = 'abcde12345';
    await expect(
      encodeTextJpeg(inputPath, jpegOutputPath, message, header),
    ).resolves.toBeUndefined();
    await expect(decodeTextJpeg(jpegOutputPath, '54321edcba')).rejects.toThrow(
      'Failed to decode message',
    );
  });
});
