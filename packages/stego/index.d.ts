/**
 * Encodes text into a PNG image using steganography
 * @param inputPath - Path to the input PNG image
 * @param outputPath - Path where the output PNG image will be saved
 * @param message - Text message to encode into the image
 * @returns Promise<void>
 */
export async function encodeTextPng(inputPath: string, outputPath: string, message: string): Promise<void>;

// Add other function declarations as they're implemented

declare const stego: {
  encodeTextPng: typeof encodeTextPng;
};

export default stego; 