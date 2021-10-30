#include "cppmath.h"
#include <math.h>
#include <limits>

double sign(double x)
{
	if (x>=0.) return 1.;
	else return -1.;
}
double adj_int(double x)
{
	double ax = fabs(x);
	double c = ceil(ax);
	if ((c-ax)>0.5) c -= 1.;
	return c*sign(x);
}
double intdiv(double x, double y)
{
	return floor(fabs(x)/fabs(y))*sign(x)*sign(y);
}