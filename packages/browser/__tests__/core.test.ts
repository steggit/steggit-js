/* eslint-disable no-underscore-dangle */
import fs from 'node:fs';
import path, { dirname } from 'node:path';
import { fileURLToPath } from 'node:url';
import { loadSteggitModule } from '../src/index';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const getInputPath = (name: string) => {
  return path.join(__dirname, '..', '__fixtures__', name);
};

describe('encode png - error handling', () => {
  it('should throw if missing arguments', async () => {
    const module = await loadSteggitModule();
    const inputPath = getInputPath('test.png');
    const input = fs.readFileSync(inputPath);
    const message = 'test';
    const missingMessageResult = module.encodeTextPng(input, '');
    expect(missingMessageResult).toEqual(1);
    const missingInputResult = module.encodeTextPng('', message);
    expect(missingInputResult).toEqual(1);
    const missingInputAndMessageResult = module.encodeTextPng('', '');
    expect(missingInputAndMessageResult).toEqual(1);
    const invalidInputResult = module.encodeTextPng(
      new File([], 'test.png'),
      message,
    );
    expect(invalidInputResult).toEqual(1);
  });

  it('should throw if input is not a PNG image', async () => {
    const module = await loadSteggitModule();
    const inputJpegPath = getInputPath('test.jpg');

    // Buffer
    const input = fs.readFileSync(inputJpegPath);
    const invalidBufferResult = await module.encodeTextPng(input, 'test');
    expect(invalidBufferResult).toEqual(1);

    // File
    const file = new File([input], 'test.jpg', { type: 'image/jpeg' });
    const invalidFileResult = await module.encodeTextPng(file, 'test');
    expect(invalidFileResult).toEqual(1);

    // String
    const inputString = fs.readFileSync(inputJpegPath, 'base64');
    const invalidStringResult = await module.encodeTextPng(inputString, 'test');
    expect(invalidStringResult).toEqual(1);
  });
});
