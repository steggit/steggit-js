CC=gcc
CFLAGS=-Wall -I/opt/homebrew/opt/jpeg-turbo/include -I/opt/homebrew/opt/libpng/include -Iinclude -g
LDFLAGS=-L/opt/homebrew/opt/jpeg-turbo/lib -L/opt/homebrew/opt/libpng/lib -ljpeg -lpng

SRC = src
OBJ = $(SRC:.c=.o)
TARGET = build/stego

# ---- Unity Test Setup ----
UNITY_DIR = third_party/unity
UNITY_SRC = $(UNITY_DIR)/src/unity.c
UNITY_INC = -I$(UNITY_DIR)/src
TEST_SRC = test/test_png.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_TARGET = build/test_runner

# ---- Build main project ----
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# ---- Build test runner ----
test: $(TEST_TARGET)
	@echo "Running tests..."
	./$(TEST_TARGET)

$(TEST_TARGET): $(OBJ) $(TEST_OBJ) $(UNITY_SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $(UNITY_INC) -o $@ $^ $(LDFLAGS)

# ---- Build object files ----
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ---- Clean all builds ----
clean:
	rm -f $(OBJ) $(TEST_OBJ) $(TARGET) $(TEST_TARGET)

# ---- Clean only test build ----
clean-test:
	rm -f $(TEST_OBJ) $(TEST_TARGET)
