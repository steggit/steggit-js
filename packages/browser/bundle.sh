#!/bin/bash

# Check if the format parameter is provided
if [ $# -ne 1 ]; then
  echo "Usage: $0 <cjs|esm>"
  exit 1
fi

FORMAT=$1

if [ "$FORMAT" != "esm" ] && [ "$FORMAT" != "cjs" ]; then
  echo "Error: Format must be either 'esm' or 'cjs'"
  exit 1
fi

if [ "$FORMAT" == "esm" ]; then
  OUTPUT_FILE="dist/index.js"
  FORMAT_FLAG="esm"
else
  OUTPUT_FILE="dist/index.cjs"
  FORMAT_FLAG="cjs"
fi

echo "Generating TypeScript declarations..."
tsc --emitDeclarationOnly --outDir dist

echo "Bundling with esbuild in $FORMAT_FLAG format..."
esbuild src/index.ts \
  --bundle \
  --outfile="$OUTPUT_FILE" \
  --format="$FORMAT_FLAG" \
  --platform=node \
  --external:dist/steggit_emcc.js

echo "Bundle created successfully: $OUTPUT_FILE"
