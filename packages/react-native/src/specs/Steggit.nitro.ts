import { type HybridObject } from 'react-native-nitro-modules';

export interface Steggit
  extends HybridObject<{
    ios: 'c++';
    android: 'c++';
  }> {
  encodeTextPng(
    inputPath: string,
    outputPath: string,
    message: string,
    header?: string,
  ): Promise<void>;
  encodeTextJpeg(
    inputPath: string,
    outputPath: string,
    message: string,
    header?: string,
  ): Promise<void>;
  decodeTextPng(inputPath: string, header?: string): Promise<string>;
  decodeTextJpeg(inputPath: string, header?: string): Promise<string>;
}
