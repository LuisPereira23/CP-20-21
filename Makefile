CC = gcc
CFLAGS = -lm -Wall -O3 -std=c99

seq: seq.c
	$(CC) -fopenmp seq.c -o seq $(CFLAGS)

parallel: parallel.c
	$(CC) -fopenmp parallel.c -o parallel $(CFLAGS)

clean:
	@rm -f seq
	@rm -f parallel
