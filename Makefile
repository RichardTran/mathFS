all: test.c
	gcc -Wall -lm test.c `pkg-config fuse --cflags --libs` -o test
