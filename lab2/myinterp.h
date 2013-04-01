#ifndef _myinterp_h_
#define _myinterp_h_

typedef enum {
    my_interp_lagrange,
    my_interp_newton
} my_interp_type;

struct {
    double * coeff;
    my_interp_type T;
    size_t size;
} typedef my_interp;

my_interp * my_interp_alloc(my_interp_type T, size_t size);

int my_interp_init(my_interp * interp, const double * xs, const double * ys, size_t size);

double my_interp_eval(const my_interp * interp, const double * xs, const double * ys, double x);

void my_interp_free(my_interp * interp);

#endif
