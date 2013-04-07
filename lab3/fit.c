#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <gsl/gsl_statistics.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit.h>

#define C(i) (gsl_vector_get(c,(i)))

int main(int argc, char ** argv) {
    int n, i;
    double size, val, dev, chisq;
    scanf("%d", &n);

    gsl_matrix * X, * cov;
    gsl_vector * y, * w, * c;
    X = gsl_matrix_alloc(n, 4);
    y = gsl_vector_alloc(n);
    w = gsl_vector_alloc(n);
    c = gsl_vector_alloc(4);
    cov = gsl_matrix_alloc(4, 4);

    for (i = 0; i < n; i++) {
        scanf("%lf %lf %lf", &size, &val, &dev);

        gsl_matrix_set(X, i, 0, 1.0);
        gsl_matrix_set(X, i, 1, size);
        gsl_matrix_set(X, i, 2, size*size);
        gsl_matrix_set(X, i, 3, size*size*size);
        gsl_vector_set(y, i, val);
        gsl_vector_set(w, i, 1.0/(dev*dev));
    }

    gsl_multifit_linear_workspace * work = gsl_multifit_linear_alloc(n, 4);
    gsl_multifit_wlinear(X, w, y, c, cov, &chisq, work);
    printf("f(x) = %g + %g*x + %g*x**2 + %g*x**3\n", C(0), C(1), C(2), C(3));

    gsl_multifit_linear_free (work);
    gsl_matrix_free (X);
    gsl_vector_free (y);
    gsl_vector_free (w);
    gsl_vector_free (c);
    gsl_matrix_free (cov);

    return 0;
}
