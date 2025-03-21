mkdir -p dist
emcc \
  ../core/src/main.c \
  ../core/src/general_utils.c \
  ../core/src/jpeg_utils.c \
  ../core/src/png_utils.c \
  -o dist/steggit_module.js \
  -I ../core/include \
  -s USE_LIBPNG=1 \
  -s USE_LIBJPEG=1 \
  -s MODULARIZE=1 \
  -s 'EXPORTED_FUNCTIONS=["_encode_png","_encode_jpeg","_decode_png","_decode_jpeg"]' \
  -s 'EXPORTED_RUNTIME_METHODS=["ccall","cwrap"]' \
  -O3