{
  "targets": [
    {
      "target_name": "steggit_core", 
      "sources": [
        "src/general_utils.c",
        "src/jpeg_utils.c",
        "src/png_utils.c",
        "src/wrapper.c",
        "src/wrapper_utils.c"
      ],
      "include_dirs": [
        "src",
        "<!(pkg-config --cflags-only-I libpng | sed 's/-I//g')",
        "<!(pkg-config --cflags-only-I libjpeg | sed 's/-I//g')"
      ],
      "cflags": [
        "-Wall",
        "-g"
      ],
      "libraries": [
        "<!(pkg-config --libs libpng libjpeg)"
      ],
    }
  ]
}