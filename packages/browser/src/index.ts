import ModuleFactory, { type StegoModule } from 'dist/steggit_emcc.js';
import { allocateMemory, freeMemory, validateInput, writeInputToFS } from './utils';

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
export async function encodeTextPng(input: File | Buffer | string, message: string, header?: string): Promise<Blob> {
  const mod = await getModule();

  if (!message || !message.length) {
    throw new Error('No message provided');
  }
  validateInput(input, 'image/png');

  const inputFilename = '/input.png';
  const outputFilename = '/output.png';

  await writeInputToFS(input, inputFilename, mod);
  const memory = allocateMemory(inputFilename, outputFilename, message, header || '', mod);
  let result = -1;
  let errorMessage = '';
  let outputBuffer: Uint8Array | null = null;
  try {
    result = mod._encode_png(memory.input, memory.output, memory.header, memory.message, memory.error);
    errorMessage = mod.UTF8ToString(memory.error);
    if (result === 0) {
      outputBuffer = mod.FS.readFile(outputFilename);
    }
  } catch (error: unknown) {
    console.error('UNHANDLED ERROR', error);
    result = -1;
    errorMessage = (error as Error)?.message || 'Unknown error';
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
    throw new Error('No message returned');
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
export async function encodeTextJpeg(input: File | Buffer | string, message: string, header?: string): Promise<Blob> {
  const mod = await getModule();

  if (!message || !message.length) {
    throw new Error('No message provided');
  }
  validateInput(input, 'image/jpeg');

  const inputFilename = '/input.jpg';
  const outputFilename = '/output.jpg';

  await writeInputToFS(input, inputFilename, mod);
  const memory = allocateMemory(inputFilename, outputFilename, message, header || '', mod);
  let result = -1;
  let errorMessage = '';
  let outputBuffer: Uint8Array | null = null;
  try {
    result = mod._encode_jpeg(memory.input, memory.output, memory.header, memory.message, memory.error);
    errorMessage = mod.UTF8ToString(memory.error);
    if (result === 0) {
      outputBuffer = mod.FS.readFile(outputFilename);
    }
  } catch (error: unknown) {
    console.error('UNHANDLED ERROR', error);
    result = -1;
    errorMessage = (error as Error)?.message || 'Unknown error';
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
    throw new Error('No message returned');
  }

  return new Blob([outputBuffer], { type: 'image/jpeg' });
}

/**
 * Decodes text from a PNG image using LSB steganography
 * @param inputPath - Path to the input PNG image
 * @param header - Header to use for the decoded message. If not provided, the default header will be used.
 * @returns Promise<string>
 */
export async function decodeTextPng(input: string, header?: string): Promise<string> {
  const mod = await getModule();
  const inputPtr = mod._malloc(input.length + 1);
  const headerPtr = mod._malloc((header?.length || 0) + 1);
  const messagePtr = mod._malloc(1024);
  const errorPtr = mod._malloc(1024);
  try {
    mod.stringToUTF8(input, inputPtr, input.length + 1);
    mod.stringToUTF8(header || '', headerPtr, header?.length || 0);
    const result = mod._decode_png(inputPtr, headerPtr, errorPtr);

    if (result > 0) {
      throw new Error(mod.UTF8ToString(errorPtr));
    }

    const message = mod.UTF8ToString(messagePtr);
    if (!message?.length) {
      throw new Error('No message found');
    }

    return message;
  } finally {
    mod._free(inputPtr);
    mod._free(headerPtr);
    mod._free(messagePtr);
    mod._free(errorPtr);
  }
}

/**
 * Decodes text from a JPEG image using DCT steganography
 * @param inputPath - Path to the input JPEG image
 * @param header - Header to use for the decoded message. If not provided, the default header will be used.
 * @returns Promise<string>
 */
export async function decodeTextJpeg(input: string, header?: string): Promise<string> {
  const mod = await getModule();
  const inputPtr = mod._malloc(input.length + 1);
  const headerPtr = mod._malloc((header?.length || 0) + 1);
  const messagePtr = mod._malloc(1024);
  const errorPtr = mod._malloc(1024);
  try {
    mod.stringToUTF8(input, inputPtr, input.length + 1);
    mod.stringToUTF8(header || '', headerPtr, header?.length || 0);
    const result = mod._decode_jpeg(inputPtr, headerPtr, errorPtr);

    if (result > 0) {
      throw new Error(mod.UTF8ToString(errorPtr));
    }

    const message = mod.UTF8ToString(messagePtr);
    if (!message?.length) {
      throw new Error('No message found');
    }

    return message;
  } finally {
    mod._free(inputPtr);
    mod._free(headerPtr);
    mod._free(messagePtr);
    mod._free(errorPtr);
  }
}


export async function createStegoApi() {
  const mod = await getModule();
  return {
    encodeTextPng: mod.cwrap('encode_png', 'number', ['number', 'number', 'number', 'number']),
    encodeTextJpeg: mod.cwrap('encode_jpeg', 'number', ['number', 'number', 'number', 'number']),
    decodeTextPng: mod.cwrap('decode_png', 'number', ['number', 'number']),
    decodeTextJpeg: mod.cwrap('decode_jpeg', 'number', ['number', 'number']),
  };
}