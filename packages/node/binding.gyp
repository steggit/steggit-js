{
  "targets": [
    {
      "target_name": "stego", 
      "sources": [
        "../core/src/general_utils.c",
        "../core/src/jpeg_utils.c",
        "../core/src/png_utils.c",
        "../core/src/wrapper.c",
        "../core/src/wrapper_utils.c"
      ],
      "include_dirs": [
        "../core/src",
        "../core/include",
        "/opt/homebrew/opt/jpeg-turbo/include",
        "/opt/homebrew/opt/libpng/include",
      ],
      "cflags": [
        "-Wall",
        "-g"
      ],
      "libraries": [
        "-L/opt/homebrew/opt/jpeg-turbo/lib",
        "-L/opt/homebrew/opt/libpng/lib",
        "-ljpeg",
        "-lpng"
      ]
    }
  ]
}