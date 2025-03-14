CC=gcc
CFLAGS=-Wall -I/opt/homebrew/opt/jpeg-turbo/include -Iinclude -g
LDFLAGS=-L/opt/homebrew/opt/jpeg-turbo/lib -ljpeg

SRC = src/main.c src/stego.c src/jpeg_utils.c
OBJ = $(SRC:.c=.o)
TARGET = build/stego

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)