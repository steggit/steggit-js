/* eslint-disable */
const env = require('dotenv');

const testEnv = env.config({ path: 'test.env' });

process.env = {
  ...(process.env || {}),
  ...testEnv.parsed,
};
