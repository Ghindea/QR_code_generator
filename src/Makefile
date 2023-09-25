CC = gcc
CFLAGS = -g -std=c11 #-fsanitize=address #-Wall
build:
	@$(CC) *.c $(CFLAGS) -o project

run: build
	@./project

valgrind: 
	valgrind --leak-check=yes --track-origins=yes ./project

clean:
	rm project QR.ppm