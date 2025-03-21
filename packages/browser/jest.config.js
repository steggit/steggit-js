module.exports = {
  testEnvironment: 'jsdom',
  testMatch: ['**/*.test.ts'],
  moduleFileExtensions: ['ts', 'js'],
  moduleNameMapper: {
    '^dist/steggit_emcc.js$': '<rootDir>/dist/steggit_emcc.js'
  },
  transform: {
    '^.+\\.tsx?$': [
      '@cloudventure/jest-transformer-esbuild'
    ]
  },
}; 