#include <string.h>
#include <stdlib.h>
#include "myinterp.h"
#include "lagrange.h"
#include "newton.h"


my_interp * my_interp_alloc(my_interp_type T, size_t size) {
    my_interp * res = (my_interp*) malloc(sizeof(my_interp));

    res->T = T;
    res->size = size;
    res->coeff = (double*) malloc((size+1) * sizeof(double));

    int i;
    for (i = 0; i < size; i++) res->coeff[i] = 0.;

    return res;
}

int my_interp_init(my_interp * interp, const double * xs, const double * ys, size_t size) {
    switch(interp->T) {
        case my_interp_lagrange:
            lagrange_interp_init(interp, xs, ys, size);
        break;
        case my_interp_newton:
            newton_interp_init(interp, xs, ys, size);
        break;
    }
    return 0;
}

double my_interp_eval(const my_interp * interp, const double * xs, const double * ys, double x) {
    switch(interp->T) {
        case my_interp_lagrange:
            return lagrange_interp_eval(interp, x);
        break;
        case my_interp_newton:
            return newton_interp_eval(interp, x);
        break;
    }
}

void my_interp_free(my_interp * interp){

    free(interp->coeff);
    free(interp);

}
