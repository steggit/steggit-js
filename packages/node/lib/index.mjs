/* eslint-disable */
import { createRequire } from 'module';

const dynamicRequire = createRequire(import.meta.url);

const stego = dynamicRequire('../build/Release/steggit_core.node');

export const { encodeTextPng } = stego;
export const { encodeTextJpeg } = stego;
export const { decodeTextPng } = stego;
export const { decodeTextJpeg } = stego;
