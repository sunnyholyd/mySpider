CC = gcc
CFLAGS = -O3 -march=core2 -Wall -g
spider.o: *.c
	$(CC) *.c -o spider.o -levent -lrt -lpthread
clean:
	-rm spider.o
