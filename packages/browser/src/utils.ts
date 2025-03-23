/* eslint-disable no-underscore-dangle */
// eslint-disable-next-line import/extensions
import { StegoModule } from 'dist/steggit_emcc.js';

interface Memory {
  input: number | null;
  output: number | null;
  message: number | null;
  header: number | null;
  error: number | null;
}

export function validateInput(input: File | Buffer | string, mimeType: string) {
  if (
    !input ||
    (typeof input === 'string' && !input.length) ||
    (input instanceof File && !input.size) ||
    (input instanceof Buffer && !input.length)
  ) {
    throw new Error('No input provided');
  }
  const extension = mimeType.split('/')[1];
  if (input instanceof File && input.type !== mimeType) {
    throw new Error(`File is not a valid ${extension} image`);
  }
  if (
    typeof input === 'string' &&
    !input.startsWith(`data:${mimeType};base64,`)
  ) {
    throw new Error(`Input string is not a valid ${extension} image`);
  }
}

export async function writeInputToFS(
  input: File | Buffer | string,
  inputFilename: string,
  mod: StegoModule,
) {
  try {
    if (input instanceof File) {
      const arrayBuffer = await input.arrayBuffer();
      const buffer = new Uint8Array(arrayBuffer);
      mod.FS.writeFile(inputFilename, buffer);
    } else if (input instanceof Buffer) {
      mod.FS.writeFile(inputFilename, input);
    } else if (typeof input === 'string') {
      if (!input.startsWith('data:')) {
        throw new Error('Input is not a data URL');
      }
      const base64Data = input.split(',')[1];
      // Convert base64 to binary
      const binaryData = Buffer.from(base64Data, 'base64');
      mod.FS.writeFile(inputFilename, binaryData);
    } else {
      throw new Error('Invalid input type');
    }
  } catch (error: unknown) {
    const errorMessage = (error as Error).message || 'Unknown error';
    throw new Error(`Failed to write input file: ${errorMessage}`);
  }
}

export function getErrorMessage(memory: Memory, mod: StegoModule) {
  if (!memory.error) {
    return '';
  }
  const errorPtr = mod.getValue(memory.error, 'i32');
  const errorStr = mod.UTF8ToString(errorPtr);
  return errorStr;
}

export function allocateMemory(
  inputFilename: string,
  outputFilename: string,
  msg: string,
  hdr: string,
  mod: StegoModule,
): Memory {
  const msgLength = msg.length ? msg.length + 1 : 1024;
  const input = mod._malloc(inputFilename.length + 1);
  const output = mod._malloc(outputFilename.length + 1);
  const message = mod._malloc(msgLength);
  const error = mod._malloc(4);
  let header: number | null = null;
  if (hdr.length) {
    header = mod._malloc(hdr.length + 1);
    mod.stringToUTF8(hdr, header, hdr.length + 1);
  }
  mod.stringToUTF8(inputFilename, input, inputFilename.length + 1);
  mod.stringToUTF8(outputFilename, output, outputFilename.length + 1);
  mod.stringToUTF8(msg, message, msgLength);
  mod.setValue(error, 0, 'i32');
  return { input, output, message, header, error };
}

export function freeMemory(memory: Memory, mod: StegoModule) {
  if (memory.input) {
    mod._free(memory.input);
  }
  if (memory.output) {
    mod._free(memory.output);
  }
  if (memory.message) {
    mod._free(memory.message);
  }
  if (memory.header) {
    mod._free(memory.header);
  }
  if (memory.error) {
    mod._free(memory.error);
  }
}
