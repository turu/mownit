CC=gcc
CFLAGS=-Wall -lm

all: gen

gen: gen.o
	$(CC) -o $@.out $^ $(CFLAGS)
	mkdir data
	./gen.out

clean:
	rm -rvf *.o
	rm -rvf *.out
	rm -rvf *.p
	rm -rvf *.dat
	rm -rvf data/
