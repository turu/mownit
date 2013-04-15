CC = gcc
CFLAGS = -Wall -lgsl -lgslcblas -lm -lrt

.PHONY = all clean zad1

all: zad1

zad1: zad1.o
	${CC} ${CFLAGS} $^ -o $@.out

clean:
	rm -rfv *.out
	rm -rfv *.o
