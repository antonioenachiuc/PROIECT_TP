CC = clang
PKG = sdl2 SDL2_image SDL2_ttf

CFLAGS = -Wall -Wextra -std=c17 $(shell pkg-config --cflags $(PKG))
LDFLAGS = $(shell pkg-config --libs $(PKG))

TARGET = pacman
SRC = main.c create_table.c 

all: $(TARGET)

$(TARGET): $(SRC)
	@$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

re: clean all