import ModuleFactory, { type StegoModule } from 'dist/steggit_emcc.js';

let moduleInstance: StegoModule | null = null;

async function getModule(): Promise<StegoModule> {
  if (!moduleInstance) {
    moduleInstance = await ModuleFactory(); // Initialize the WASM module
  }
  return moduleInstance;
}

/**
 * Encodes text into a PNG image using LSB steganography
 * @param inputPath - Path to the input PNG image
 * @param outputPath - Path where the output PNG image will be saved
 * @param message - Text message to encode into the image
 * @param header - Header to use for the encoded message. If not provided, the default header will be used.
 * @returns Promise<void>
 */
export async function encodeTextPng(input: string, output: string, message: string, header?: string): Promise<void> {
  const mod = await getModule();
  const inputPtr = mod._malloc(input.length + 1);
  const outputPtr = mod._malloc(output.length + 1);
  const messagePtr = mod._malloc(message.length + 1);
  const errorPtr = mod._malloc(1024);
  const headerPtr = mod._malloc((header?.length || 0) + 1);
  try {
    mod.stringToUTF8(input, inputPtr, input.length + 1);
    mod.stringToUTF8(output, outputPtr, output.length + 1);
    mod.stringToUTF8(header || '', headerPtr, header?.length || 0);
    mod.stringToUTF8(message, messagePtr, message.length + 1);
    const result = mod._encode_png(inputPtr, outputPtr, headerPtr, messagePtr, errorPtr);

    if (result > 0) {
      throw new Error(mod.UTF8ToString(errorPtr));
    }
  } finally {
    mod._free(inputPtr);
    mod._free(outputPtr);
    mod._free(headerPtr);
    mod._free(messagePtr);
    mod._free(errorPtr);
  }
}

/**
 * Encodes text into a JPEG image using DCT steganography
 * @param inputPath - Path to the input JPEG image
 * @param outputPath - Path where the output JPEG image will be saved
 * @param message - Text message to encode into the image
 * @param header - Header to use for the encoded message. If not provided, the default header will be used.
 * @returns Promise<void>
 */
export async function encodeTextJpeg(input: string, output: string, message: string, header?: string): Promise<void> {
  const mod = await getModule();
  const inputPtr = mod._malloc(input.length + 1);
  const outputPtr = mod._malloc(output.length + 1);
  const messagePtr = mod._malloc(message.length + 1);
  const errorPtr = mod._malloc(1024);
  const headerPtr = mod._malloc((header?.length || 0) + 1);
  try {
    mod.stringToUTF8(input, inputPtr, input.length + 1);
    mod.stringToUTF8(output, outputPtr, output.length + 1);
    mod.stringToUTF8(header || '', headerPtr, header?.length || 0);
    mod.stringToUTF8(message, messagePtr, message.length + 1);
    const result = mod._encode_jpeg(inputPtr, outputPtr, headerPtr, messagePtr, errorPtr);

    if (result > 0) {
      throw new Error(mod.UTF8ToString(errorPtr));
    }
  } finally {
    mod._free(inputPtr);
    mod._free(outputPtr);
    mod._free(headerPtr);
    mod._free(messagePtr);
    mod._free(errorPtr);
  }
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