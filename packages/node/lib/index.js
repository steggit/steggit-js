/* eslint-disable */
const path = require('path');
const load = require('node-gyp-build');

const steggit = load(path.join(__dirname, '..'));

// CommonJS exports
module.exports = {
  encodeTextPng: steggit.encodeTextPng,
  encodeTextJpeg: steggit.encodeTextJpeg,
  decodeTextPng: steggit.decodeTextPng,
  decodeTextJpeg: steggit.decodeTextJpeg,
};

// TypeScript CommonJS interop
module.exports.encodeTextPng = steggit.encodeTextPng;
module.exports.encodeTextJpeg = steggit.encodeTextJpeg;
module.exports.decodeTextPng = steggit.decodeTextPng;
module.exports.decodeTextJpeg = steggit.decodeTextJpeg;
