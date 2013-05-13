CC=gcc
CFLAGS=-Wall -lgsl -lrt

.PHONY=zad clean

zad: zad.c
	$(CC) $(CFLAGS) $< -o $@.out

clean:
	rm -rvf *.out
	rm -rvf *.o
