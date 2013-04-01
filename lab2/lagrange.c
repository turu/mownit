#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "lagrange.h"
#include "myinterp.h"

//calculates a polynomial of a form: PRODUCT(j, 0, n, w/o i) of (x - xj)/(xi - xj)
double * L(int i, const double * x, double y, int n, double * l)
{
    int j, k;
    double denom = 1., phi = y, p, o;
    l[0] = 1.;
    for(j = 1; j < n; j++) l[j] = 0;

    for(j = 0; j < n; j++)
    {
        if(j != i)
        {
            denom *= (x[i] - x[j]);
            p = o = 1.;
            for(k = 1; k < n; k++)
            {
                o = l[k];
                l[k] = o - x[j] * p;
                p = o;
            }
        }
    }
    phi /= denom;
    for(j = 0; j < n; j++)
    {
        l[j] *= phi;
    }
    return l;
}

//returns a table containing coefficients of a interpolation polynomial
void lagrange(const double * x, const double * y, int n, double * c)
{
    int i, j;
    double * l;
    for(i = 0; i < n; i++) c[i] = 0;

    l = (double*) malloc(n * sizeof(double));

    //for each of the interpolation nodes...
    for(i = 0; i < n; i++)
    {
        //calculate L(i) polynomial of a form: yi * PRODUCT(j, 0, n, w/o i) of (x - xj)/(xi - xj)
        L(i, x, y[i], n, l);
        //... and add it to the result
        for(j = 0; j < n; j++)
            c[j] += l[j];

        memset(l, 0, n * sizeof(double));
    }

    free(l);
}


int lagrange_interp_init(my_interp * interp, const double * xs, const double * ys, size_t size) {
    lagrange(xs, ys, size, interp->coeff);

    return 0;
}

double lagrange_interp_eval(const my_interp * interp, double x) {
    double res = 0.;

    int i = 0;
    while(i < interp->size) {
        res = res * x + interp->coeff[i];
        i++;
    }

    return res;
}

