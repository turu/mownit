#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "randplot.h"

void printHelp() {
    printf("Usage:\n-n <number>  - number of points\n-a <number>  - min x\n-b <number>\
  - min y\n-c <number>  - max x\n-d <number>  - max y\n");
    printf("Output: n, equally spaced, randomly distributed (y-axis) points lying within bounding rectangle\n");
}

int main(int argc, char ** argv) {
    int n;
    double * x, * y;
    double xmin, ymin, xmax, ymax;
    char c;
    char * optvalue;

    while ((c = getopt(argc, argv, "n:a:b:c:d:")) != -1) {
        switch(c) {
            case 'n':
                optvalue = optarg;
                if (optvalue != 0) {
                    n = atoi(optvalue);
                }
            break;
            case 'a':
                optvalue = optarg;
                if (optvalue != 0) {
                    xmin = atof(optvalue);
                }
            break;
            case 'b':
                optvalue = optarg;
                if (optvalue != 0) {
                    ymin = atof(optvalue);
                }
            break;
            case 'c':
                optvalue = optarg;
                if (optvalue != 0) {
                    xmax = atof(optvalue);
                }
            break;
            case 'd':
                optvalue = optarg;
                if (optvalue != 0) {
                    ymax = atof(optvalue);
                }
            break;
            default:
                printHelp();
                return 0;
            break;
        }
    }

    if(n == 0) {
        printHelp();
        return 0;
    }

    x = (double*) malloc(n * sizeof(double));
    y = (double*) malloc(n * sizeof(double));
    randyplot2d(xmin, ymin, xmax, ymax, n, x, y);
    printf("%d\n", n);
    int i;
    for(i = 0; i < n; i++) {
        printf("%lf %lf\n", x[i], y[i]);
    }

    free(x);
    free(y);

    return 0;
}
