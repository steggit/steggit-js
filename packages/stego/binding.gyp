{
  "targets": [
    {
      "target_name": "stego", 
      "sources": [
        "src/general_utils.c",
        "src/jpeg_utils.c",
        "src/png_utils.c",
        "src/wrapper.c",
        "src/wrapper_utils.c"
      ],
      "include_dirs": [
        "src",
        "/opt/homebrew/opt/jpeg-turbo/include",
        "/opt/homebrew/opt/libpng/include",
        "include"
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