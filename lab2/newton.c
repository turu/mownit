#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "newton.h"
#include "myinterp.h"


void addPoly(my_interp * a, my_interp * b){
    if(a->size == b->size) {
        int i = a->size;
        while(i--) {
            a->coeff[i] += b->coeff[i];
        }
    }
}

my_interp * copy(my_interp * p){
    my_interp * ret = my_interp_alloc(p->T, p->size);

    int i = p->size;
    while(i--) {
        ret->coeff[i] = p->coeff[i];
    }

    return ret;
}

void mulmono(my_interp * p, double c){
    int i= p->size;

    my_interp * cpy = copy(p);

    while(i-- > 1){
        p->coeff[i] = c * cpy->coeff[i];
        p->coeff[i] += cpy->coeff[i-1];
    }

    p->coeff[0] = c * cpy->coeff[0];
}

void mulc(my_interp * p, double c){
    int i = p->size;

    while(i--) {
        p->coeff[i] *= c;
    }
}

void newton(const double * x, const double * y, int num, my_interp * p)
{
    int roots = p->size;
    int i = roots;
    int j = roots;

    double ** tab = malloc(roots * sizeof(double*));
    while(j--) {
        tab[j] = malloc( (j+1)* sizeof(double) );
    }

    j=roots;
    while(j--) {
        tab[j][0] = y[j];
    }

    j=1;
    while(j < roots) {
        int k=1;
        while(k <= j) {
            tab[j][k] = (tab[j][k-1]-tab[j-1][k-1]) / (x[j] - x[j-k]);
            k++;
        }
        j++;
    }

    my_interp * tmp = my_interp_alloc(p->T, p->size);
    my_interp * n = my_interp_alloc(p->T, p->size);

    n->coeff[0]=1;
    i=0;

    while(i < roots) {
        my_interp * m = copy(n);
        mulc(n, tab[i][i]);
        addPoly(tmp, n);
        j = roots;
        n = m;
        mulmono(n, -x[i]);
        i++;
    }

    i = roots;
    while(i--) {
        p->coeff[i] = tmp->coeff[i];
    }

    my_interp_free(tmp);
    my_interp_free(n);
}


int newton_interp_init(my_interp * interp, const double * xs, const double * ys, size_t size) {
    newton(xs, ys, size, interp);

    return 0;
}

double newton_interp_eval(const my_interp * interp, double x) {
    double res = 0.;

    int i = interp->size - 1;
    while(i < interp->size) {
        res = res * x + interp->coeff[i];
        i--;
    }

    return res;
}
