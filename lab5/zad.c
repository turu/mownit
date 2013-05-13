#include<stdio.h>
#include<sys/times.h>
#include<gsl/gsl_roots.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include<unistd.h>

#define ELO -1
#define EPS 1e-15
#define TIMES 1000000


void printStats(clock_t real, struct tms *start, struct tms *end) {
    static long clktck = 0;
    if(clktck == 0)
        clktck = sysconf(_SC_CLK_TCK);
    printf("real: %7.2fs\n",real/(double)clktck);
    printf("user: %7.2fs\n",(end->tms_utime-start->tms_utime)/(double)clktck);
    printf("sys: %7.2fs\n",(end->tms_stime-start->tms_stime)/(double)clktck);
}

int findRootBucket(gsl_root_fsolver * s, int max_it, double * value) {
    int it = 0, status;
    double x_lo, x_hi;
    double r;

    do {
        gsl_root_fsolver_iterate(s);
        r = gsl_root_fsolver_root(s);
        x_lo = gsl_root_fsolver_x_lower(s);
        x_hi = gsl_root_fsolver_x_upper(s);
        status = gsl_root_test_interval(x_lo, x_hi, EPS, 0);
    } while (status == GSL_CONTINUE && ++it < max_it);

    *value = r;

    return it;
}

int findRootDiff(gsl_root_fdfsolver * s, int max_it, double x_st, double * value) {
    int it = 0, status;
    double x0, x = x_st;

    do {
        gsl_root_fdfsolver_iterate (s);
        x0 = x;
        x = gsl_root_fdfsolver_root (s);
        status = gsl_root_test_delta (x, x0, EPS, 0);
    } while (status == GSL_CONTINUE && ++it < max_it);

    *value=x;

    return it;
}

double f1(double x, void * param) {
    return x*x + 12.0*x + 3.0;
}

double f3(double x, void * param) {
    if (x > 2.0) return -1.0;
    return x*x - 4.0*x + 3.0;
}

double df3(double x, void * param) {
    if (x > 2.0) return .0;
    return 2.0 * x - 4.0;
}

void fdf3(double x, void * param, double * f, double * df) {
    *f =  f3(x, NULL);
    *df = df3(x, NULL);
}

double f2(double x, void * param) {
    return x*x + 4.0*x + 4.0;
}

double df1(double x, void * param) {
    return 2.0 * x + 12.0;
}

double df2(double x, void * param) {
    return 2.0 * x + 4.0;
}

void fdf1(double x, void * param, double * f, double * df) {
    *f =  x*x + 12.0*x + 3.0;
    *df = 2.0 * x + 12.0;
}

void fdf2(double x, void * param, double * f, double * df) {
    *f =  x*x + 4.0*x + 4.0;
    *df = 2.0 * x + 4.0;
}

void simple(int max_it, const gsl_root_fsolver_type * B) {
    gsl_function F1,F2;
    F1.function = &f1;
    F2.function = &f2;

    printf("F. kwadratowa, pierwiastki jednokrotne\n");
    struct tms str, end;
    int i;
    int iters;
    double result;
    clock_t strt = times(&str);
    gsl_root_fsolver * slvr = gsl_root_fsolver_alloc(B);

    for(i = 0; i < TIMES; i++) {
        gsl_root_fsolver_set(slvr, &F1, -6,50);
        iters = findRootBucket(slvr, max_it, &result);
    }

    clock_t endt = times(&end);

    printStats(endt-strt, &str, &end);

    printf("iteracje:\t%d\n",iters);

    printf("Podwójny pierwiastek, zły przedział\n");
    strt = times(&str);
    for(i=0;i<TIMES;i++){
        gsl_root_fsolver_set (slvr, &F2, 0, 4);
        iters = findRootBucket(slvr, max_it, &result);
    }
    endt = times(&end);
    printStats(endt-strt,&str,&end);
    printf("iteracje:\t%d\n",iters);
    gsl_root_fsolver_free (slvr);

}

void diff(int max_it,const gsl_root_fdfsolver_type * D) {
    gsl_function_fdf F1,F2;
    F1.f = &f1;
    F1.df = &df1;
    F1.fdf = &fdf1;
    F2.f = &f3;
    F2.df = &df3;
    F2.fdf = &fdf3;

    gsl_root_fdfsolver* slvr = gsl_root_fdfsolver_alloc(D);
    struct tms str, end;
    int iters,i;
    double result;
    printf("F. kwadratowa, pierwiastki jednokrotne\n");
    clock_t strt = times(&str);
    for(i=0;i<TIMES;i++){
        gsl_root_fdfsolver_set (slvr,&F1,20);
        iters = findRootDiff(slvr,max_it,20,&result);
    }
    clock_t endt = times(&end);
    printStats(endt-strt,&str,&end);
    printf("iteracje:\t%d\n",iters);
    printf("Pochodna sie zeruje na przedziale\n");
    strt = times(&str);
    for(i=0;i<TIMES;i++){
        gsl_root_fdfsolver_set (slvr,&F2,20);
        iters = findRootDiff(slvr,max_it,20,&result);
    }
    endt = times(&end);
    printStats(endt-strt,&str,&end);
    printf("iteracje:\t%d\n",iters);
    gsl_root_fdfsolver_free (slvr);
}

int main(int argc, char ** argv) {
    if (argc != 2) {
        printf("Podaj numer metody od 1 do 6.\n");
        return 1;
    }

    int num = atoi(argv[1]);
    int iters = 250;

    switch (num) {
        case 1:
            printf("Regula falsi\n");
            simple(iters, gsl_root_fsolver_falsepos);
            break;
        case 2:
            printf("Metoda Brenta\n");
            simple(iters, gsl_root_fsolver_brent);
            break;
        case 3:
            printf("Polowienie\n");
            simple(iters, gsl_root_fsolver_bisection);
            break;
        case 4:
            printf("Metoda Newtona\n");
            diff(iters, gsl_root_fdfsolver_newton);
            break;
        case 5:
            printf("Metoda Steffensona\n");
            diff(iters, gsl_root_fdfsolver_steffenson);
            break;
        case 6:
            printf("Metoda siecznych\n");
            diff(iters, gsl_root_fdfsolver_secant);
            break;
        default:
            printf("Nie umiesz czytac.");
            break;
    }

    return 0;
}
