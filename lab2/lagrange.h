#ifndef _lagrange_h_
#define _lagrange_h_

#include "myinterp.h"

int lagrange_interp_init(my_interp * interp, const double * xs, const double * ys, size_t size);

double lagrange_interp_eval(const my_interp * interp, double x);

#endif
