CC = gcc
CFLAGS = -g -std=c11 -fsanitize=address #-Wall
# set MAKEPNG = true in order to produce PNG files using stb_image_write
MAKEPNG = false

build:
ifeq ($(MAKEPNG), true)
	@$(CC) -DQRCODE_MAKE_PNG *.c dep/stb_image_write.c $(CFLAGS) -o project
else
	@$(CC) *.c $(CFLAGS) -o project
endif

run: build
	@./project

valgrind: 
	valgrind --leak-check=yes --track-origins=yes ./project

clean:
	rm project QR.ppm
