const config = {
  extensionsToTreatAsEsm: ['.ts'],
  verbose: true,
  preset: 'ts-jest/presets/default-esm',
  testEnvironment: 'node',
  transform: {
    '^.+\\.tsx?$': [
      'jest-esbuild',
      {
        format: 'esm',
        target: 'esnext',
      },
    ],
  },
};

export default config;
