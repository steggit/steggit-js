const config = {
  testEnvironment: 'jsdom',
  testMatch: ['**/*.test.ts'],
  moduleFileExtensions: ['ts', 'js'],
  setupFilesAfterEnv: ['./jest.setup.js'],
  preset: 'ts-jest/presets/default-esm',
  moduleNameMapper: {
    '^build/steggit_emcc.js$': '<rootDir>/build/steggit_emcc.js',
  },
  transform: {
    '^.+\\.tsx?$': [
      'jest-esbuild',
      {
        format: 'esm',
        target: 'esnext',
      },
    ],
  },
  extensionsToTreatAsEsm: ['.ts'],
};

export default config;
