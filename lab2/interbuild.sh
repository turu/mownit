#!/usr/bin/sh

gcc -Wall -I ../commons -lgsl -lrt interpolate.c myinterp.c lagrange.c newton.c -o interpolate.out
