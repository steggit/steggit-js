const getGlobalSecret = () => {
  const secret = process.env.GLOBAL_SECRET;
  if (!secret) {
    throw new Error('GLOBAL_SECRET is not set');
  }
  return secret;
};

export default getGlobalSecret;
