/* eslint-disable */
import { createRequire } from 'module';

const require = createRequire(import.meta.url);

const stego = require('./build/Release/stego');

export const { encodeTextPng } = stego;
export const { encodeTextJpeg } = stego;
export const { decodeTextPng } = stego;
export const { decodeTextJpeg } = stego;
