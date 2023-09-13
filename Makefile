CC = gcc
#CFLAGS = -g -Wall -std=c11
build:
	@$(CC) *.c $(CFLAGS) -o project

run: build
	@./project
	@ @xdg-open QR.ppm
valgrind: 
	valgrind --leak-check=yes --track-origins=yes ./project

clean:
	rm project