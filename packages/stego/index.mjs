import { createRequire } from 'module';
const require = createRequire(import.meta.url);

const stego = require('./build/Release/stego');

export const encodeTextPng = stego.encodeTextPng; 