CC=gcc
CFLAGS=-Wall -I/opt/homebrew/opt/jpeg-turbo/include
LDFLAGS=-L/opt/homebrew/opt/jpeg-turbo/lib -ljpeg

SRC=src/main.c
TARGET=build/stego

all: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)