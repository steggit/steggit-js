/* eslint-disable */
import { createRequire } from 'module';
import path from 'path';
const require = createRequire(import.meta.url);

const load = require('node-gyp-build');
const steggit = load(path.resolve(path.dirname(import.meta.url), '..'));

export const encodeTextPng = steggit.encodeTextPng;
export const encodeTextJpeg = steggit.encodeTextJpeg;
export const decodeTextPng = steggit.decodeTextPng;
export const decodeTextJpeg = steggit.decodeTextJpeg;
