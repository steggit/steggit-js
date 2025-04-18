declare module '@steggit-js/browser/build/steggit_emcc.js' {
  interface StegoModule {
    _encode_png: (
      inputPtr: number | null,
      outputPtr: number | null,
      messagePtr: number | null,
      headerPtr: number | null,
      errorPtrPtr: number | null,
    ) => number;
    _encode_jpeg: (
      inputPtr: number | null,
      outputPtr: number | null,
      messagePtr: number | null,
      headerPtr: number | null,
      errorPtrPtr: number | null,
    ) => number;
    _decode_png: (
      inputPtr: number | null,
      headerPtr: number | null,
      errorPtrPtr: number | null,
    ) => number;
    _decode_jpeg: (
      inputPtr: number | null,
      headerPtr: number | null,
      errorPtrPtr: number | null,
    ) => number;

    cwrap: (
      funcName: string,
      returnType: string,
      argTypes: string[],
    ) => (...args: unknown[]) => unknown;
    UTF8ToString: (ptr: number) => string;
    _malloc: (size: number) => number;
    _free: (ptr: number) => void;
    stringToUTF8: (
      str: string,
      outPtr: number,
      maxBytesToWrite: number,
    ) => void;
    getValue: (ptr: number, type: string) => number;
    setValue: (ptr: number, index: number, value: string) => void;
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
