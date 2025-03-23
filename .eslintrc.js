module.exports = {
  plugins: ['@typescript-eslint'],
  extends: [
    'airbnb-base',
    'plugin:@typescript-eslint/strict-type-checked',
    'plugin:prettier/recommended',
  ],
  parser: '@typescript-eslint/parser',
  parserOptions: {
    ecmaVersion: 2022,
    project: ['./tsconfig.eslint.json', './packages/*/tsconfig.json'],
    tsconfigRootDir: __dirname,
  },
  ignorePatterns: ['**/*/dist/**', '.eslintrc.js'],
  root: true,
  rules: {
    'import/prefer-default-export': [
      'off',
      {
        except: ['**/index.ts', '**/index.tsx'],
      },
    ],
    'import/extensions': [
      'error',
      'never',
      {
        json: 'always',
      },
    ],
    'import/no-unresolved': 'off',
    'max-len': ['warn', 150],
    'prettier/prettier': 'warn',
    curly: ['error', 'all'],
  },
  overrides: [
    {
      files: ['**/*.test.ts'],
      env: {
        jest: true,
      },
    },
  ],
};
