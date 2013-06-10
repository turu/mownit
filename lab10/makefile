CC=gcc
CFLAGS=-Wall -lm

all: gen

gen: gen.o
	$(CC) -o $@.out $^ $(CFLAGS)
	mkdir data
	./gen.out
	gnuplot animate.p
	gifsicle -O2 animate.gif -o animate.gif

clean:
	rm -rvf *.o
	rm -rvf *.out
	rm -rvf *.p
	rm -rvf *.dat
	rm -rvf data/
