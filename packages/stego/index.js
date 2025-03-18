const stego = require('./build/Release/stego');

// CommonJS exports
module.exports = {
  encodeTextPng: stego.encodeTextPng
};

// Also support named exports for TypeScript CommonJS interop
module.exports.encodeTextPng = stego.encodeTextPng;

// Add other exported functions here as they're implemented
// module.exports.functionName = functionName; 