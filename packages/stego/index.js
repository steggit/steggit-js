const stego = require('./build/Release/stego');

// CommonJS exports
module.exports = {
  encodeTextPng: stego.encodeTextPng,
  encodeTextJpeg: stego.encodeTextJpeg,
  decodeTextPng: stego.decodeTextPng,
  decodeTextJpeg: stego.decodeTextJpeg
};

// TypeScript CommonJS interop
module.exports.encodeTextPng = stego.encodeTextPng;
module.exports.encodeTextJpeg = stego.encodeTextJpeg;
module.exports.decodeTextPng = stego.decodeTextPng;
module.exports.decodeTextJpeg = stego.decodeTextJpeg;
