/* eslint-disable @typescript-eslint/no-var-requires */
/* eslint-disable import/no-extraneous-dependencies */
// const { sentryEsbuildPlugin } = require('@sentry/esbuild-plugin');

/**
 *
 * @param {{ service: { custom: { sentry: import('@sentry/esbuild-plugin').SentryEsbuildPluginOptions }}}} serverless
 * @returns {unknown[]}
 */
module.exports = () => {
  return {
    bundle: true,
    minify: true,
    // TODO: Revisit using ESM in future. Currently Jest and AWS Lambda support not sufficient
    format: 'cjs',
    treeShaking: true,
    write: true,
    minifyWhitespace: true,
    minifyIdentifiers: true,
    target: 'node22',
    sourcemap: true,
    exclude: ['@aws-sdk/*', 'aws-sdk', 'sharp'],
    resolveExtensions: ['.ts', '.js', '.mjs'],
    // plugins: [
    //   sentryEsbuildPlugin({
    //     ...sentryOptions,
    //     disable: process.env.DISABLE_SENTRY || process.env.DISABLE_SOURCEMAPS,
    //   }),
    // ],
  };
};
