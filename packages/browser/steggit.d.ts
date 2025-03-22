declare module 'dist/steggit_emcc.js' {
  interface StegoModule {
    _encode_png: (inputPtr: number, outputPtr: number, headerPtr: number, messagePtr: number, errorPtr: number) => number;
    _encode_jpeg: (inputPtr: number, outputPtr: number, headerPtr: number, messagePtr: number, errorPtr: number) => number;
    _decode_png: (inputPtr: number, headerPtr: number, errorPtr: number) => number;
    _decode_jpeg: (inputPtr: number, headerPtr: number, errorPtr: number) => number;

    cwrap: (
      funcName: string,
      returnType: string,
      argTypes: string[]
    ) => (...args: any[]) => any;
    UTF8ToString: (ptr: number) => string;
    _malloc: (size: number) => number;
    _free: (ptr: number) => void;
    stringToUTF8: (str: string, outPtr: number, maxBytesToWrite: number) => void;
    FS: {
      writeFile: (path: string, data: Uint8Array | string) => void;
      readFile: (path: string) => Uint8Array;
      unlink: (path: string) => void;
    };
  }

  const StegoModuleFactory: () => Promise<StegoModule>;
  export default StegoModuleFactory;
  export { type StegoModule };
}