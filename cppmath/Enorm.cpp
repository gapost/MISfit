/* enorm.c -- compute euclidean norm of vector */
#include <math.h>
#include "cppmath.h"

template<class T>
class EuclidianNorm
/* computes euklidian norm of a series of values 
   algorithm from GSL blas
*/
{
	protected:
		T scale,ssq;
	public:
		EuclidianNorm() : scale(0.), ssq(1.) {}
		~EuclidianNorm() {}
		void operator+=(const T& x)
		{
			if (x != 0.0) {
				T ax = fabs(x);
				
				if (scale < ax) {
					ssq = 1.0 + ssq * (scale / ax) * (scale / ax);
					scale = ax;
				} else {
					ssq += (ax / scale) * (ax / scale);
				}
			}
		}
		T norm() { return scale*sqrt(ssq); }
};

double enorm(int n, CArray1D& x)
{
	EuclidianNorm<double> N;

	for(int i=0; i<n; i++) N += x[i];

	return N.norm();
}

/* compute row norm for column c from row r to row m */

double rownorm(int m, int r, int c, CArray2D& x)
{
	EuclidianNorm<double> N;

    for (int i = r; i < m; i++) N += x[i][c];

	return N.norm();
}

double colnorm(int m, int r, int c, CArray2D& x)
{
	EuclidianNorm<double> N;

    for (int i = c; i < m; i++) N += x[r][i];

	return N.norm();
}

