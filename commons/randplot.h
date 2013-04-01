#ifndef _randplot_h_
#define _randplot_h_

#include <time.h>
#include <math.h>
#include <stdlib.h>

//Generates n, equally spaced, randomly distributed (y-axis) points lying within bounding rect.
void randyplot2d(double xmin, double ymin, double xmax, double ymax, int n, double * xs, double * ys) {
    srand(time(NULL));
    double x = xmin;
    double y;
    double step = (xmax - xmin) / (n - 1);
    int h = ymax - ymin;
    int i = 0;
    while(n--) {
        y = ymin + (double)(rand() % (h * 1000)) / 1000.;
        xs[i] = x;
        ys[i] = y;
        x += step;
        i++;
    }
}

#endif
