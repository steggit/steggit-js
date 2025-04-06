/* eslint-disable */
import { createRequire } from 'module';
import path from 'path';
import { fileURLToPath } from 'url';

const dynamicRequire = createRequire(import.meta.url);
const load = dynamicRequire('node-gyp-build');

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const steggit = load(path.resolve(__dirname, '..'));

export const encodeTextPng = steggit.encodeTextPng;
export const encodeTextJpeg = steggit.encodeTextJpeg;
export const decodeTextPng = steggit.decodeTextPng;
export const decodeTextJpeg = steggit.decodeTextJpeg;
