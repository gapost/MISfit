#include "cppmath.h"
#include <math.h>
#include <limits>

double midpnt(INTEG_FUNC func, double a, double b, int n, void* par /* = NULL */)
/*
This routine computes the nth stage of refinement of an extended midpoint rule. func is input
as a pointer to the function to be integrated between limits a and b, also input. When called with
n=1, the routine returns the crudest estimate of the integral f(x)dx. Subsequent calls with n=2,3,...
(in that sequential order) will improve the accuracy of s by adding (2/3) × 3n-1 additional
interior points. s should not be modi.ed between sequential calls.
*/
{
	double x,tnm,sum,del,ddel;
	static double s;
	int it,j;

	if (n == 1) {
		return (s=(b-a)*(*func)(0.5*(a+b),par));
	} else {
		for(it=1,j=1;j<n-1;j++) it *= 3;
		tnm=it;
		del=(b-a)/(3.0*tnm);
		ddel=del+del; //The added points alternate in spacing between del and ddel. 
		x=a+0.5*del;
		sum=0.0;
		for (j=1;j<=it;j++) {
			sum += (*func)(x,par);
			x += ddel;
			sum += (*func)(x,par);
			x += del;
		}
		s=(s+(b-a)*sum/tnm)/3.0; //The new sum is combined with the old integral to give a refined integral. 
		return s;
	}
}

#define JMAX1 50
#define JMAXP1 (JMAX1+1)
#define K 5

int polint(double* xac, double* yac, int n, double x, double& y, double& dy);

double qromo(INTEG_FUNC func, double a, double b,
			double (*choose)(INTEG_FUNC, double, double, int, void*), 
			double EPS /* = 1.e-6 */, void* par /* = NULL */)
/*
Romberg integration on an open interval. Returns the integral of the function func from a to b,
using any specified integrating function choose and Romberg’s method. Normally choose will
be an open formula, not evaluating the function at the endpoints. It is assumed that choose
triples the number of steps on each call, and that its error series contains only even powers of
the number of steps. The routines midpnt, midinf, midsql, midsqu, midexp, are possible
choices for choose. The parameters have the same meaning as in qromb.
*/
{
	int j;
	double ss,dss,h[JMAXP1+1],s[JMAXP1];
	h[1]=1.0;
	for (j=1;j<=JMAX1;j++) {
		s[j]=(*choose)(func,a,b,j,par);
		if (j >= K) {
			polint(&h[j-K],&s[j-K],K,0.0,ss,dss);
			if (fabs(dss) <= EPS*fabs(ss) && j>K) return ss;
		}
		h[j+1]=h[j]/9.0; //This is where the assumption of step tripling and an even error series is used. 
	}
	//nrerror("Too many steps in routing qromo");
	return 0.0; //Never get here.
}


void gauleg(double x1, double x2, CArray1D& x_c, CArray1D& w_c, int n)
/*
Given the lower and upper limits of integration x1 and x2, and given n, this routine returns
arrays x[1..n] and w[1..n] of length n, containing the abscissas and weights of the Gauss-
Legendre n-point quadrature formula.
*/
{
	TNT::Fortran_Array1D<double> x(x_c);
	TNT::Fortran_Array1D<double> w(w_c);

	int m,j,i;
	double z1,z,xm,xl,pp,p3,p2,p1; //High precision is a good idea for this routine.
	m=(n+1)/2; //The roots are symmetric in the interval, so
	//we only have to finnd half of them. 
	xm=0.5*(x2+x1);
	xl=0.5*(x2-x1);
	for (i=1;i<=m;i++) { //Loop over the desired roots.
		z=cos(PI*(i-0.25)/(n+0.5));
		//Starting with the above approximation to the ith root, we enter the main loop of
		//refinement by Newton’s method.
		do {
			p1=1.0;
			p2=0.0;
			for (j=1;j<=n;j++) { //Loop up the recurrence relation to get the
				//Legendre polynomial evaluated at z. 
				p3=p2;
				p2=p1;
				p1=((2.0*j-1.0)*z*p2-(j-1.0)*p3)/j;
			}
			//p1 is now the desired Legendre polynomial. We next compute pp, its derivative,
			//by a standard relation involving also p2, the polynomial of one lower order.
			pp=n*(z*p1-p2)/(z*z-1.0);
			z1=z;
			z=z1-p1/pp; //Newton’s method.
		} while (fabs(z-z1) > DBL_EPSILON);
		x(i)=xm-xl*z; //Scale the root to the desired interval,
		x(n+1-i)=xm+xl*z; //and put in its symmetric counterpart.
		w(i)=2.0*xl/((1.0-z*z)*pp*pp); //Compute the weight
		w(n+1-i)=w(i); //and its symmetric counterpart.
	}
}


double trapzd(INTEG_FUNC func, double a, double b, int n, void* par /* = NULL */)
/*
This routine computes the nth stage of refinement of an extended trapezoidal rule. func is input
as a pointer to the function to be integrated between limits a and b, also input. When called with
n=1, the routine returns the crudest estimate of .b
a f(x)dx. Subsequent calls with n=2,3,...
(in that sequential order) will improve the accuracy by adding 2n-2 additional interior points.
*/
{
	float x,tnm,sum,del;
	static float s;
	int it,j;
	if (n == 1) {
		return (s=0.5*(b-a)*(func(a,par)+func(b,par)));
	} else {
		for (it=1,j=1;j<n-1;j++) it <<= 1;
		tnm=it;
		del=(b-a)/tnm; //This is the spacing of the points to be added.
		x=a+0.5*del;
		for (sum=0.0,j=1;j<=it;j++,x+=del) sum += func(x,par);
		s=0.5*(s+(b-a)*sum/tnm); //This replaces s by its refined value.
		return s;
	}
}

#define JMAX2 20
#define JMAXP2 (JMAX2+1)

/*
Here EPS is the fractional accuracy desired, as determined by the extrapolation error estimate;
JMAX limits the total number of steps; K is the number of points used in the extrapolation.
*/
double qromb(INTEG_FUNC func, double a, double b,
			 double EPS /* = 1.e-6 */, void* par /* = NULL */)
/*
Returns the integral of the function func from a to b. Integration is performed by Romberg’s
method of order 2K, where, e.g., K=2 is Simpson’s rule.
*/
{
	double ss,dss;
	double s[JMAXP2],h[JMAXP2+1]; //These store the successive trapezoidal approximations
	//and their relative stepsizes. 
	int j;
	h[1]=1.0;
	for (j=1;j<=JMAX2;j++) {
		s[j]=trapzd(func,a,b,j,par);
		if (j >= K) {
			polint(&h[j-K],&s[j-K],K,0.0,ss,dss);
			if (fabs(dss) <= EPS*fabs(ss) && j>K) return ss;
		}
		h[j+1]=0.25*h[j];
		/*
		This is a key step: The factor is 0.25 even though the stepsize is decreased by only
		0.5. This makes the extrapolation a polynomial in h2 as allowed by equation (4.2.1),
		not just a polynomial in h.
		*/
	}
	//nrerror("Too many steps in routine qromb");
	return 0.0; //Never get here.
}