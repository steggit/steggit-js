/* eslint-disable no-underscore-dangle */
/* eslint-disable import/extensions */
import ModuleFactory, {
  type StegoModule,
} from '@steggit-js/browser/build/steggit_emcc.js';
import {
  allocateMemory,
  freeMemory,
  getErrorMessage,
  validateInput,
  writeInputToFS,
} from './utils';

let moduleInstance: StegoModule | null = null;

async function getModule(): Promise<StegoModule> {
  if (!moduleInstance) {
    moduleInstance = await ModuleFactory(); // Initialize the WASM module
  }
  return moduleInstance;
}

/**
 * Encodes text into a PNG image using LSB steganography
 * @param input - Input image file
 * @param message - Text message to encode into the image
 * @param header - Header to use for the encoded message. If not provided, the default header will be used.
 * @returns Promise<Blob>
 */
export async function encodeTextPng(
  input: File | string,
  message: string,
  header?: string,
): Promise<Blob> {
  const mod = await getModule();

  if (!message || !message.length) {
    throw new Error('No message provided');
  }
  validateInput(input, 'image/png');

  const inputFilename = '/input.png';
  const outputFilename = '/output.png';

  await writeInputToFS(input, inputFilename, mod);
  const memory = allocateMemory(
    inputFilename,
    outputFilename,
    message,
    header || '',
    mod,
  );
  let result = -1;
  let errorMessage = '';
  let outputBuffer: Uint8Array | null = null;
  try {
    result = mod._encode_png(
      memory.input,
      memory.output,
      memory.message,
      memory.header,
      memory.error,
    );
    errorMessage = getErrorMessage(memory, mod);
    if (result === 0) {
      outputBuffer = mod.FS.readFile(outputFilename);
    }
  } catch (error: unknown) {
    console.error('UNHANDLED ERROR', error);
    result = -1;
    errorMessage = (error as Error).message || 'Unknown error';
  } finally {
    freeMemory(memory, mod);
    mod.FS.unlink(inputFilename);
    if (outputBuffer?.length) {
      mod.FS.unlink(outputFilename);
    }
  }

  if (result !== 0) {
    throw new Error(errorMessage);
  }
  if (!outputBuffer?.length) {
    throw new Error('No file returned');
  }

  return new Blob([outputBuffer], { type: 'image/png' });
}

/**
 * Encodes text into a JPEG image using DCT steganography
 * @param input - Input image file
 * @param message - Text message to encode into the image
 * @param header - Header to use for the encoded message. If not provided, the default header will be used.
 * @returns Promise<Blob>
 */
export async function encodeTextJpeg(
  input: File | string,
  message: string,
  header?: string,
): Promise<Blob> {
  const mod = await getModule();

  if (!message || !message.length) {
    throw new Error('No message provided');
  }
  validateInput(input, 'image/jpeg');

  const inputFilename = '/input.jpg';
  const outputFilename = '/output.jpg';

  await writeInputToFS(input, inputFilename, mod);
  const memory = allocateMemory(
    inputFilename,
    outputFilename,
    message,
    header || '',
    mod,
  );
  let result = -1;
  let errorMessage = '';
  let outputBuffer: Uint8Array | null = null;
  try {
    result = mod._encode_jpeg(
      memory.input,
      memory.output,
      memory.message,
      memory.header,
      memory.error,
    );
    errorMessage = getErrorMessage(memory, mod);
    if (result === 0) {
      outputBuffer = mod.FS.readFile(outputFilename);
    }
  } catch (error: unknown) {
    console.error('UNHANDLED ERROR', error);
    result = -1;
    errorMessage = (error as Error).message || 'Unknown error';
  } finally {
    freeMemory(memory, mod);
    mod.FS.unlink(inputFilename);
    if (outputBuffer?.length) {
      mod.FS.unlink(outputFilename);
    }
  }

  if (result !== 0) {
    throw new Error(errorMessage);
  }
  if (!outputBuffer?.length) {
    throw new Error('No file returned');
  }

  return new Blob([outputBuffer], { type: 'image/jpeg' });
}

/**
 * Decodes text from a PNG image using LSB steganography
 * @param input - Input image file
 * @param header - Header to use for the decoded message. If not provided, the default header will be used.
 * @returns Promise<string>
 */
export async function decodeTextPng(
  input: File | string,
  header?: string,
): Promise<string> {
  const mod = await getModule();
  validateInput(input, 'image/png');
  const inputFilename = '/input.png';

  await writeInputToFS(input, inputFilename, mod);
  const memory = allocateMemory(inputFilename, '', '', header || '', mod);

  let result = -1;
  let errorMessage = '';
  let output = '';
  try {
    result = mod._decode_png(memory.input, memory.header, memory.error);
    errorMessage = getErrorMessage(memory, mod);
    if (result) {
      output = mod.UTF8ToString(result);
    }
  } catch (error) {
    console.error('UNHANDLED ERROR', error);
    errorMessage = (error as Error).message || 'Unknown error';
  } finally {
    freeMemory(memory, mod);
    mod.FS.unlink(inputFilename);
  }

  if (errorMessage) {
    throw new Error(errorMessage);
  }
  if (!output.length) {
    throw new Error('No message found');
  }

  return output;
}

/**
 * Decodes text from a JPEG image using DCT steganography
 * @param input - Input image file
 * @param header - Header to use for the decoded message. If not provided, the default header will be used.
 * @returns Promise<string>
 */
export async function decodeTextJpeg(
  input: File | string,
  header?: string,
): Promise<string> {
  const mod = await getModule();
  validateInput(input, 'image/jpeg');
  const inputFilename = '/input.jpg';

  await writeInputToFS(input, inputFilename, mod);
  const memory = allocateMemory(inputFilename, '', '', header || '', mod);

  let result = -1;
  let errorMessage = '';
  let output = '';
  try {
    result = mod._decode_jpeg(memory.input, memory.header, memory.error);
    errorMessage = getErrorMessage(memory, mod);
    if (result) {
      output = mod.UTF8ToString(result);
    }
  } catch (error: unknown) {
    console.error('UNHANDLED ERROR', error);
    result = -1;
    errorMessage = (error as Error).message || 'Unknown error';
  } finally {
    freeMemory(memory, mod);
    mod.FS.unlink(inputFilename);
  }

  if (errorMessage) {
    throw new Error(errorMessage);
  }
  if (!output.length) {
    throw new Error('No message found');
  }

  return output;
}

export async function loadSteggitModule() {
  const mod = await getModule();
  return {
    encodeTextPng: mod.cwrap('encode_png', 'number', [
      'number',
      'number',
      'number',
      'number',
    ]),
    encodeTextJpeg: mod.cwrap('encode_jpeg', 'number', [
      'number',
      'number',
      'number',
      'number',
    ]),
    decodeTextPng: mod.cwrap('decode_png', 'number', ['number', 'number']),
    decodeTextJpeg: mod.cwrap('decode_jpeg', 'number', ['number', 'number']),
  };
}
