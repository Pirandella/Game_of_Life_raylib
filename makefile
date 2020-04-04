TARGET = gameOfLife

all:
	gcc main.c -o $(TARGET) -lraylib -Wall
