CC=gcc
CFLAGS=-Wall -I/opt/homebrew/opt/jpeg-turbo/include -I/opt/homebrew/opt/libpng/include -Iinclude -g
LDFLAGS=-L/opt/homebrew/opt/jpeg-turbo/lib -L/opt/homebrew/opt/libpng/lib -ljpeg -lpng

SRC = src/main.c src/jpeg_utils.c src/file_utils.c src/png_utils.c
OBJ = $(SRC:.c=.o)
TARGET = build/stego

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)