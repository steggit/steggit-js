/**
 * Encodes text into a PNG image using LSB steganography
 * @param inputPath - Path to the input PNG image
 * @param outputPath - Path where the output PNG image will be saved
 * @param message - Text message to encode into the image
 * @param header - Header to use for the encoded message. If not provided, the default header will be used.
 * @returns Promise<void>
 */
export async function encodeTextPng(
  inputPath: string,
  outputPath: string,
  message: string,
  header?: string,
): Promise<void>;

/**
 * Decodes text from a PNG image using LSB steganography
 * @param inputPath - Path to the input PNG image
 * @param header - Header to use for the decoded message. If not provided, the default header will be used.
 * @returns Promise<string>
 */
export async function decodeTextPng(
  inputPath: string,
  header?: string,
): Promise<string>;

/**
 * Encodes text into a JPEG image using DCT steganography
 * @param inputPath - Path to the input JPEG image
 * @param outputPath - Path where the output JPEG image will be saved
 * @param message - Text message to encode into the image
 * @param header - Header to use for the encoded message. If not provided, the default header will be used.
 * @returns Promise<void>
 */
export async function encodeTextJpeg(
  inputPath: string,
  outputPath: string,
  message: string,
  header?: string,
): Promise<void>;

/**
 * Decodes text from a JPEG image using DCT steganography
 * @param inputPath - Path to the input JPEG image
 * @param header - Header to use for the decoded message. If not provided, the default header will be used.
 * @returns Promise<string>
 */
export async function decodeTextJpeg(
  inputPath: string,
  header?: string,
): Promise<string>;

declare const stego: {
  encodeTextPng: typeof encodeTextPng;
  encodeTextJpeg: typeof encodeTextJpeg;
  decodeTextPng: typeof decodeTextPng;
  decodeTextJpeg: typeof decodeTextJpeg;
};

export default stego;
