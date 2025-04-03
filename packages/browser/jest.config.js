module.exports = {
  testEnvironment: 'jsdom',
  testMatch: ['**/*.test.ts'],
  moduleFileExtensions: ['ts', 'js'],
  setupFilesAfterEnv: ['./jest.setup.js'],
  moduleNameMapper: {
    '^dist/steggit_emcc.js$': '<rootDir>/dist/steggit_emcc.js',
  },
  transform: {
    '^.+\\.tsx?$': ['jest-esbuild'],
  },
};
