CC = gcc
#CFLAGS = -g -Wall -std=c11
build:
	@$(CC) *.c $(CFLAGS) -o project

run: build
	@./project
	
valgrind: 
	valgrind --leak-check=yes --track-origins=yes ./project

clean:
	rm project QR.ppm