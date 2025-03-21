import { createRequire } from 'module';
const require = createRequire(import.meta.url);

const stego = require('./build/Release/stego');

export const encodeTextPng = stego.encodeTextPng;
export const encodeTextJpeg = stego.encodeTextJpeg;
export const decodeTextPng = stego.decodeTextPng;
export const decodeTextJpeg = stego.decodeTextJpeg;
