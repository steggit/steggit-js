import { randomBytes } from 'node:crypto';

const generateUserSecret = () => {
  return randomBytes(32).toString('hex');
};

export default generateUserSecret;
