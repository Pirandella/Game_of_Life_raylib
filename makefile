TARGET = gameOfLife

CC = gcc
CFLAGS = -Wall -lraylib

all:
	$(CC) main.c -o $(TARGET) $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(TARGET)