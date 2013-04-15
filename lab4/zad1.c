#include <stdlib.h>
#include <stdio.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_statistics.h>
#include <math.h>

#define TRIES 100
#define MAXSTEPS 100

double f(double x) {
    return 7*x*x - 4*x + 3;
}


double g(double x) {
    return  sin(666*x) + 1;
}


double h(double x) {
    return  1. / sqrt(1. - x);
}


double k(double x){
    return  sin(x*2)/x;
}

double rectIntegration( double (*fn)(double x), double from, double to, int steps ) {
    if (from >= to){
        printf("%f must be smaller than %f", from, to);
    }

    double delta = (double) (to - from) / steps;

    int i;
    double sum = 0.0;
    for(i = 1; i<=steps; i++)
    {
        sum += fn(delta * i) * delta;
    }

    return sum;
}

int computeNumberOfSteps(double (*f)(double), double a, double b, double error) {
    double prev = rectIntegration(f, a, b, 1);
    double cur  = rectIntegration(f, a, b, 2);
    int res = 2;

    while (fabs(cur - prev) > error*error) {
        res++;
        prev = cur;
        cur = rectIntegration(f, a, b, res);
    }

    return res;
}

struct timeval stop, start;

int main(int argc, char ** argv) {
    double error = atof(argv[1]);
    double a = atof(argv[2]);
    double b = atof(argv[3]);
    int i;

    printf("Podstawowe calkowanie.\n");

    int steps = computeNumberOfSteps(f, a, b, error);
    printf("Dla zadanego bledu potrzeba wykonac %d krokow, wynik = %lf\n", steps,
           rectIntegration(f, a, b, steps));

    double times[TRIES];

    for (i = 0; i < TRIES; i++) {
        gettimeofday(&start, NULL);
        rectIntegration(f, a, b, steps);
        gettimeofday(&stop, NULL);

        times[i] = (double)(stop.tv_usec - start.tv_usec);
    }
    printf("Wyliczenie calki f metoda prostokatow zajelo srednio %f usec\n\n", gsl_stats_mean(times, 1, TRIES));

    gsl_function F;
    F.function = &f;
    size_t gsteps;
    double result, acterror;
    gsl_integration_qng(&F, a, b, 0, error, &result, &acterror, &gsteps);
    printf("Wyliczenie calki f metoda QNG dalo wynik %f i wymagalo %d krokow\n", result, gsteps);

    for (i = 0; i < TRIES; i++) {
        gettimeofday(&start, NULL);
        gsl_integration_qng(&F, a, b, error, 0, &result, &acterror, &gsteps);
        gettimeofday(&stop, NULL);

        times[i] = (double)(stop.tv_usec-start.tv_usec);
    }
    printf("Zajelo srednio %f usec\n", gsl_stats_mean(times, 1, TRIES));

    gsl_integration_workspace * w = gsl_integration_workspace_alloc(MAXSTEPS);

    printf("\nCalkowanie adaptacyjne\n");
    F.function = &g;
    gettimeofday(&start, NULL);
    gsl_integration_qag(&F, a, b, error, 0, MAXSTEPS, 1, w, &result, &acterror);
    gettimeofday(&stop, NULL);
    printf("Funkcja QAG:           %.10lf\n", result);
    printf("Zajelo %f usec\n", (double)(stop.tv_usec-start.tv_usec));
    gettimeofday(&start, NULL);
    printf("Metoda prostokatow:    %.10lf\n", rectIntegration(k, a, b, MAXSTEPS));
    gettimeofday(&stop, NULL);
    printf("Zajelo %f usec\n", (double)(stop.tv_usec-start.tv_usec));

    printf("\nCalkowanie z osobliwosciami\n");
    F.function = &h;
    gettimeofday(&start, NULL);
    gsl_integration_qags(&F, a, b, error, 0, MAXSTEPS, w, &result, &acterror);
    gettimeofday(&stop, NULL);
    printf("Funkcja QAGS:          %.10lf\n", result);
    printf("Zajelo %f usec\n", (double)(stop.tv_usec-start.tv_usec));
    gettimeofday(&start, NULL);
    printf("Metoda prostokatow:    %.10lf\n", rectIntegration(h, a, b, MAXSTEPS));
    gettimeofday(&stop, NULL);
    printf("Zajelo %f usec\n", (double)(stop.tv_usec-start.tv_usec));

    printf("\nCalkowanie dla funkcji oscylacyjnych\n");
    F.function = &g;
    printf("Funkcja QAWO:          %.10lf\n", result);
    printf("Metdoa prostokatow:    %.10lf\n", rectIntegration(g, 0, 1, MAXSTEPS));

    return 0;
}
