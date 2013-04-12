#include <stdio.h>
#include <math.h>
#include <gsl/gsl_integration.h>
#include <time.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/resource.h>

int main(int argc, char ** argv)
{
    double precision = atof(argv[1]);
    double a = atof(argv[2]);
    double b = atof(argv[3]);

    gsl_integration_workspace * w = gsl_integration_workspace_alloc (1000);

    double result, error;
    double alpha = 1.0;

    gsl_function F;
    F.function = &exp;
    F.params = &alpha;

    gsl_integration_qags(&F, a, b, precision, 3, 1000, w, &result, &error);

    printf ("result          = % .18f\n", result);
    printf ("estimated error = % .18f\n", error);
    printf ("intervals =  %d\n", w->size);

    gsl_integration_workspace_free(w);

    return 0;
}
