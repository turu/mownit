#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>
#include "myinterp.h"
#include "timemeas.h"

void printHelp() {
    printf("Usage:\n-f <filename>  - file to which should output data be written\n-m <mode> - what algorithm to use.\
 Can be GSL_LAGRANGE, GSL_CSPLINE, GSL_AKIM, MY_LAGRANGE, MY_NEWTON\n");
    printf("Input:\n <n>  - number of nodes\nn lines containing - <x> <y>  - coordinates of the nodes");
    printf("Output: n, equally spaced, randomly distributed (y-axis) points lying within bounding rectangle\n");
}

int parseMode(char * modeString) {
    if(!strcmp(modeString, "GSL_LAGRANGE")) {
        return 1;
    } else if(!strcmp(modeString, "GSL_CSPLINE")) {
        return 2;
    } else if(!strcmp(modeString, "GSL_AKIMA")) {
        return 3;
    } else if(!strcmp(modeString, "MY_LAGRANGE")) {
        return 4;
    } else if(!strcmp(modeString, "MY_NEWTON")) {
        return 5;
    } else {
        return 0;
    }
}

void solveGSL(int n, double * xs, double * ys, FILE * file, int mode) {
    gsl_interp_type * T;
    switch(mode) {
        case 1:
            T = (gsl_interp_type*) gsl_interp_polynomial;
        break;
        case 2:
            T = (gsl_interp_type*) gsl_interp_cspline;
        break;
        case 3:
            T = (gsl_interp_type*) gsl_interp_akima;
        break;
    }

    resetTimer();
    gsl_interp_accel * accel = gsl_interp_accel_alloc();
    gsl_interp * interp = gsl_interp_alloc(T, n);
    gsl_interp_init(interp, xs, ys, n);
    double elapsed;
    elapsed = getMillisElapsed();

    printf("GSL interpolation object of type %s initialized in %.20lfms\n", gsl_interp_name(interp), elapsed);

    double x = xs[0];
    double step = 0.01;
    int yrSize = (double)(xs[n-1] - xs[0]) / step;
    double yr[yrSize];

    //printf("SSSSS %d\n", yrSize);

    int i = 0;
    while(x <= xs[n-1]) {
        yr[i] = gsl_interp_eval(interp, xs, ys, x, accel);

        x += step;
        i++;
    }
    elapsed = getMillisElapsed();

    printf("Points evaluated in %.20lfms\n", elapsed);

    x = xs[0];
    for (i = 0; i < yrSize; i++) {
        fprintf(file, "%f %f\n", x, yr[i]);
        x += step;
    }
}

void solveMy(int n, double * xs, double * ys, FILE * file, int mode) {
    my_interp_type T;
    switch(mode) {
        case 4:
            T = my_interp_lagrange;
        break;
        case 5:
            T = my_interp_newton;
        break;
    }

    resetTimer();
    my_interp * interp = my_interp_alloc(T, n);
    my_interp_init(interp, xs, ys, n);
    double elapsed;
    elapsed = getMillisElapsed();

    printf("My interpolation object initialized in %.20lfms\n", elapsed);

    double x = xs[0];
    double step = 0.01;
    int yrSize = (double)(xs[n-1] - xs[0]) / step;
    double yr[yrSize];

    //printf("SSSSS %d\n", yrSize);

    int i = 0;
    while(x <= xs[n-1]) {
        yr[i] = my_interp_eval(interp, xs, ys, x);
        //printf("%f %f\n", x,my_interp_eval(interp, xs, ys, x));

        x += step;
        i++;
    }
    elapsed = getMillisElapsed();

    printf("Points evaluated in %.20lfms\n", elapsed);

    x = xs[0];
    for (i = 0; i < yrSize; i++) {
        fprintf(file, "%f %f\n", x, yr[i]);
        x += step;
    }
}

void solve(int n, double * xs, double * ys, FILE * file, int mode) {
    if(mode < 4) { //GSL modes
        solveGSL(n, xs, ys, file, mode);
    } else { //My modes
        solveMy(n, xs, ys, file, mode);
    }
}

int main(int argc, char ** argv) {
    char filename[30];
    char modeString[20];
    int mode;

    char c;
    char * optvalue;

    if(argc == 0) {
        printHelp();
        return 0;
    }
    while ((c = getopt(argc, argv, "f:m:")) != -1) {
        switch(c) {
            case 'f':
                optvalue = optarg;
                if (optvalue != 0) {
                    strcpy(filename, optvalue);
                }
            break;
            case 'm':
                optvalue = optarg;
                if (optvalue != 0) {
                    strcpy(modeString, optvalue);
                }
            break;
            default:
                printHelp();
                return 0;
            break;
        }
    }
    mode = parseMode(modeString);

    if(mode == 0) {
        printHelp();
        return 0;
    }

    FILE * outFile = fopen(filename, "w");

    int n, i;
    double * xs, *ys;

    scanf("%d", &n);
    xs = (double*) malloc(n * sizeof(double));
    ys = (double*) malloc(n * sizeof(double));

    for(i = 0; i < n; i++) {
        scanf("%lf %lf", &xs[i], &ys[i]);
    }
    printf("%d points read.\n", i);

    solve(n, xs, ys, outFile, mode);

    fclose(outFile);

    free(xs);
    free(ys);

    return 0;
}
