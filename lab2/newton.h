#ifndef _newton_h_
#define _newton_h_

#include "myinterp.h"

int newton_interp_init(my_interp * interp, const double * xs, const double * ys, size_t size);

double newton_interp_eval(const my_interp * interp, double x);

#endif
