#include <cstdio>
#include <gsl/gsl_ieee_utils.h>

int main() {

    //Zad 2
    printf("Zad 2:\n");
    float num = 1;
    while(num > 0) {
        gsl_ieee_fprintf_float(stdout, &num);
        num /= 2.;
        putc('\n', stdout);
    }

    printf("------------------------------\nZad 3:\n");
    float eps = 1;
    while(1 + eps > 1) {
        gsl_ieee_fprintf_float(stdout, &eps);
        eps /= 2.;
        putc('\n', stdout);
    }

    printf("------------------------------\nZad 4:\n");
    double d = 1./3.;
    float f = 1./3.;
    double df = (double) f;

    printf("float f = ");
    gsl_ieee_fprintf_float(stdout, &f);
    putc('\n', stdout);

    printf("double d = ");
    gsl_ieee_fprintf_double(stdout, &d);
    putc('\n', stdout);

    printf("(double) f = ");
    gsl_ieee_fprintf_double(stdout, &df);
    putc('\n', stdout);

}
