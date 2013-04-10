#include <stdio.h>
#include <math.h>
#include <gsl/gsl_integration.h>
#include <time.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/resource.h>

void printStats(int i, struct rusage* r2, struct rusage* r1, struct timeval* t2, struct timeval* t1) {
	printf("Punkt kontrolny %d\n",i);
	printf("Aktualne dane.\nPomiar czasu\n");
	printf("rzeczywisty: %ld[s] %ld[us]\n",(long int)t2->tv_sec,(long int)t2->tv_usec);
	printf("uzytkownika: %ld[s] %ld[us]\n",(long int)r2->ru_utime.tv_sec,(long int)r2->ru_utime.tv_usec);
	printf("systemowy: %ld[s] %ld[us]\n",(long int)r2->ru_stime.tv_sec,(long int)r2->ru_stime.tv_usec);
	printf("\n\n");

	if (i!=1) {
		printf("Odniesienie do punktu kontrolnego %d\n",i-1);
		printf("Czasy wykonania:\n");
        printf("rzeczywisty: %lf[s]\n", (double)(t2->tv_sec*1000000 + t2->tv_usec - t1->tv_sec*1000000 - t1->tv_usec)/1000000);
        printf("uzytkownika: %lf[s]\n", (double)(r2->ru_utime.tv_sec*1000000 + r2->ru_utime.tv_usec - r1->ru_utime.tv_sec*1000000 -
				r1->ru_utime.tv_usec) / 1000000);
        printf("systemowy: %lf[s]\n", (double)(r2->ru_stime.tv_sec*1000000 + r2->ru_stime.tv_usec - r1->ru_stime.tv_sec*1000000 -
                r1->ru_stime.tv_usec) / 1000000);
		printf("\n\n");
	}
}

double rect_integration( double (*fn)(double x), double from, double to, int steps )
{
    if (from >= to)
    {
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

double x_2(double x)
{
    return x*x;
}

int main()
{
    struct rusage p1r, p2r;
    struct timeval p1t, p2t;

    gettimeofday(&p1t, NULL);
    getrusage(RUSAGE_SELF, &p1r);
    printStats(1, &p1r, NULL, &p1t, NULL);

    printf("%lf\n", rect_integration( &x_2, 0, 10, 1000) );

    gettimeofday(&p2t, NULL);
    getrusage(RUSAGE_SELF, &p2r);
    printStats(2, &p2r, &p1r, &p2t, &p1t);

    gsl_integration_workspace * w = gsl_integration_workspace_alloc (1000);

    double result, error;
    double expected = (double) 1000.0/3.0;
    double alpha = 1.0;

    gsl_function F;
    F.function = &x_2;
    F.params = &alpha;

    gsl_integration_qags(&F, 0, 10, 0, 1e-7, 1000, w, &result, &error);

    printf ("result          = % .18f\n", result);
    printf ("exact result    = % .18f\n", expected);
    printf ("estimated error = % .18f\n", error);
    printf ("actual error    = % .18f\n", result - expected);
    printf ("intervals =  %d\n", w->size);

    gsl_integration_workspace_free(w);

    return 0;
}
