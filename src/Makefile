CC = gcc
CFLAGS = -g -std=c11 -fsanitize=address #-Wall
build:
	@$(CC) *.c $(CFLAGS) -o qr

run: build
	@./qr

valgrind: 
	valgrind --leak-check=yes --track-origins=yes ./qr

clean:
	rm qr QR.ppm