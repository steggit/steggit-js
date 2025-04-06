/* eslint-disable */
import { createRequire } from 'node:module';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const dynamicRequire = createRequire(import.meta.url);
const load = dynamicRequire('node-gyp-build');

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const steggit = load(path.resolve(__dirname, '..'));

export const { encodeTextPng, encodeTextJpeg, decodeTextPng, decodeTextJpeg } =
  steggit;
