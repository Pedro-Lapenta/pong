# Makefile para projeto Raylib

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Para Windows, use:
# LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

# Para macOS, use:
# LIBS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

TARGET = pong
SOURCES = pong.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run