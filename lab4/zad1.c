#include <stdlib.h>
#include <stdio.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_statistics.h>

#define TRIES 100
#define MAXSTEPS 100

double f(double x){
    return 3*(x*x-2*x);
}


double g(double x){
    return  cos(100*x);
}


double h(double x){
    return  log(x) / sqrt(x);
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

    while (fabs(cur - prev) > error) {
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

    int steps = computeNumberOfSteps(f, a, b, error);
    printf("Dla zadanego bledu potrzeba wykonac %d krokow, wynik = %lf\n", steps,
           rectIntegration(f, a, b, steps));

    double times[TRIES];
    int l = TRIES;

    while (l--) {
        gettimeofday(&start, NULL);
        rectIntegration(f, a, b, steps);
        gettimeofday(&stop, NULL);
        times[l]=(double)(stop.tv_usec-start.tv_usec);
    }
    printf("Wyliczenie calki f metoda prostokatow zajelo srednio %f usec\n\n", gsl_stats_mean(times, 1, TRIES));

    gsl_function F;
    F.function = &f;
    size_t gsteps;
    double result, acterror;
    gsl_integration_qng(&F, a, b, 0, error, &result, &acterror, &gsteps);
    printf("Wyliczenie calki f metoda QNG dalo wynik %f i wymagalo %d krokow\n", result, gsteps);

    l = TRIES;
    while (l--) {
        gettimeofday(&start, NULL);
        gsl_integration_qng(&F, 0, 1, error, 0, &result, &acterror, &gsteps);
        gettimeofday(&stop, NULL);
        times[l]=(double)(stop.tv_usec-start.tv_usec);
    }
    printf("Zajelo srednio %f usec\n",gsl_stats_mean(times, 1, TRIES));
    printf("Wynik dokladny = %f\n\n", -2.0);

    F.function = &k;
    printf("Calkowanie adaptacyjne\n");
    gsl_integration_workspace * w = gsl_integration_workspace_alloc(MAXSTEPS);
    gsl_integration_qag(&F, 0.01, 100, error, 0, MAXSTEPS, 1, w, &result, &acterror);
    printf("Dla ilosci krokow:\t%d\n", MAXSTEPS);
    printf("gsl(QAG):\t%.36lf\tkroków: %d\n", result, MAXSTEPS);
    printf("trapestry:\t%.36lf\tkroków: %d\n", rectIntegration(k, 0.01, 100, MAXSTEPS), MAXSTEPS);
    printf("wolfram\t\t%f\n\n",1.54838);

    F.function = &h;
    printf("Calkowanie z osobliwosciami\n");
    gsl_integration_qags(&F, 0, 1, error, 0, MAXSTEPS, w, &result, &acterror);
    printf("Dla ilosci krokow:\t%d\n", MAXSTEPS);
    printf("gsl(QAGS):\t%.36lf\tkroków: %d\n",result, MAXSTEPS);
    printf("trapestry:\t%.36lf\tkroków: %d\n", rectIntegration(h,0,1,MAXSTEPS),MAXSTEPS);
    printf("wolfram\t\t%f\n\n",-4.0);

    F.function = &g;
    printf("Calkowanie dla funkcji oscylacyjnych\n");
    //gsl_integration_qawo(&F, 0, 1, ERROR, 0, POINTS, w,&result, &error);
    gsl_integration_qawo_table *table = gsl_integration_qawo_table_alloc(100, 1, GSL_INTEG_COSINE, MAXSTEPS);
    gsl_integration_qawo(&F, 0, error, 0, MAXSTEPS, w, table, &result, &acterror);
    printf("Dla ilosci krokow:\t%d\n",MAXSTEPS);
    printf("gsl(QAGS):\t%.36lf\tkroków: %d\n",result,MAXSTEPS);
    printf("trapestry:\t%.36lf\tkroków: %d\n",rectIntegration(g,0,1,MAXSTEPS),MAXSTEPS);
    printf("wolfram\t\t%f",-.00506366);

    return 0;
}
