#include "cppmath.h"
#include <math.h>
#include <cassert>

int polint(double* xac, double* yac, int n, double x, double& y, double& dy)
/*
Given arrays xa[1..n] and ya[1..n], and given a value x, this routine returns a value y, and
an error estimate dy. If P(x) is the polynomial of degree N - 1 such that P(xai) = yai, i =
1, . . . , n, then the returned value y = P(x).
*/
{
	int i,m,ns=1;
	double den,dif,dift,ho,hp,w;

	TNT::Fortran_Array1D<double> xa(n,xac);
	TNT::Fortran_Array1D<double> ya(n,yac);
	TNT::Fortran_Array1D<double> c(n);
	TNT::Fortran_Array1D<double> d(n);

	dif=fabs(x-xa(1));
	for (i=1;i<=n;i++) { //Here we find the index ns of the closest table entry,
		if ( (dift=fabs(x-xa(i))) < dif) {
			ns=i;
			dif=dift;
		}
		c(i)=ya(i); //and initialize the tableau of c’s and d’s.
		d(i)=ya(i);
	}
	y=ya(ns--); //This is the initial approximation to y.
	for (m=1;m<n;m++) { //For each column of the tableau,
		for (i=1;i<=n-m;i++) { //we loop over the current c’s and d’s and update them. 
			ho=xa(i)-x;
			hp=xa(i+m)-x;
			w=c(i+1)-d(i);
			if ( (den=ho-hp) == 0.0) return CPPMATH_ERR_INTERP;
			//This error can occur only if two input xa’s are (to within roundo.) identical.
			den=w/den;
			d(i)=hp*den; //Here the c’s and d’s are updated.
			c(i)=ho*den;
		}
		y += (dy=(2*ns < (n-m) ? c(ns+1) : d(ns--)));
		/*After each column in the tableau is completed, we decide which correction, c or d,
		we want to add to our accumulating value of y, i.e., which path to take through the
		tableau-forking up or down. We do this in such a way as to take the most "straight
		line" route through the tableau to its apex, updating ns accordingly to keep track of
		where we are. This route keeps the partial approximations centered (insofar as possible)
		on the target x. The last dy added is thus the error indication.*/
	}
	return CPPMATH_OK;
}


template class CSpliner<float>;
template class CSpliner<double>;

template<class T>
CSpliner<T>::CSpliner(TNT::Array1D<T>& ax, TNT::Array1D<T>& ay,
		int isnatural/* = 1*/, T yp1 /*= 0.*/, T ypn /*= 0.*/) 
: x(ax), y(ay) 
{
	InitSpline(isnatural,yp1,ypn); 
}

template<class T>
CSpliner<T>::CSpliner(int n, T* ax, T* ay,
		int isnatural/* = 1*/, T yp1 /*= 0.*/, T ypn /*= 0.*/) 
: x(n,ax), y(n,ay) 
{
	InitSpline(isnatural,yp1,ypn); 
}


template<class T>
void CSpliner<T>::InitSpline(int isnatural, T yp1, T ypn)
/*
void spline(float x[], float y[], int n, float yp1, float ypn, float y2[])
Given arrays x[1..n] and y[1..n] containing a tabulated function, i.e., yi = f(xi), with
x1 < x2 < .. . < xN, and given values yp1 and ypn for the first derivative of the interpolating
function at points 1 and n, respectively, this routine returns an array y2[1..n] that contains
the second derivatives of the interpolating function at the tabulated points xi. If yp1 and/or
ypn are equal to 1x10^30 or larger, the routine is signaled to set the corresponding boundary
condition for a natural spline, with zero second derivative on that boundary.
*/
{
	assert(x.dim()>0);
	int i,k,n=x.dim();
	T p,qn,sig,un;
	TNT::Array1D<T> u(n-1);
	y2 = TNT::Array1D<T>(n);

	if (isnatural) //The lower boundary condition is set either to be "natural"
		y2[0]=u[0]=0.0;
	else { //or else to have a specified first derivative.
		y2[0] = -0.5;
		u[0]=(3.0/(x[1]-x[0]))*((y[1]-y[0])/(x[1]-x[0])-yp1);
	}
	
	for (i=1;i<n-1;i++) { //This is the decomposition loop of the tridiagonal algorithm.
		//y2 and u are used for temporary
		//storage of the decomposed
		//factors.
		sig=(x[i]-x[i-1])/(x[i+1]-x[i-1]);
		p=sig*y2[i-1]+2.0;
		y2[i]=(sig-1.0)/p;
		u[i]=(y[i+1]-y[i])/(x[i+1]-x[i]) - (y[i]-y[i-1])/(x[i]-x[i-1]);
		u[i]=(6.0*u[i]/(x[i+1]-x[i-1])-sig*u[i-1])/p;
	}
	if (isnatural) //The upper boundary condition is set either to be "natural"
		qn=un=0.0;
	else { //or else to have a specified first derivative.
		qn=0.5;
		un=(3.0/(x[n-1]-x[n-2]))*(ypn-(y[n-1]-y[n-2])/(x[n-1]-x[n-2]));
	}
	y2[n-1]=(un-qn*u[n-2])/(qn*y2[n-2]+1.0);
	for (k=n-2;k>=0;k--) //This is the backsubstitution loop of the tridiagonal algorithm.
		y2[k]=y2[k]*y2[k+1]+u[k];
}

template<class T>
T CSpliner<T>::Spline(T xi)
/*
void splint(float xa[], float ya[], float y2a[], int n, float x, float *y)
Given the arrays xa[1..n] and ya[1..n], which tabulate a function (with the xai’s in order),
and given the array y2a[1..n], which is the output from spline above, and given a value of
x, this routine returns a cubic-spline interpolated value y.
*/
{
	//void nrerror(char error_text[]);
	int klo,khi,k,n=x.dim();
	T h,b,a,yi;
	
	/*
	We will find the right place in the table by means of
	bisection. This is optimal if sequential calls to this
	routine are at random values of x. If sequential calls
	are in order, and closely spaced, one would do better
	to store previous values of klo and khi and test if
	they remain appropriate on the next call.
	*/
	
	klo=0;
	khi=n-1;
	while (khi-klo > 1) {
		k=(khi+klo) >> 1;
		if (x[k] > xi) khi=k;
		else klo=k;
	} //klo and khi now bracket the input value of x.
	h=x[khi]-x[klo];
	//if (h == 0.0) nrerror("Bad xa input to routine splint"); //The xa’s must be distinct.
	assert(h!=0.0);
	a=(x[khi]-xi)/h;
	b=(xi-x[klo])/h; //Cubic spline polynomial is now evaluated.
	yi=a*y[klo]+b*y[khi]+((a*a*a-a)*y2[klo]+(b*b*b-b)*y2[khi])*(h*h)/6.0;
	return yi;
}

template<class T>
CSpliner2D<T>::CSpliner2D(int n, int m, T* ax1, T* ax2, T* ay)
: x1(n,ax1), x2(m,ax2), Spliners1D(n), y1(n) 
{
	TNT::Array2D<T> Y(n,m,ay);
	for(int i=0; i<n; i++) Spliners1D[i] = new CSpliner<T>(m,ax2,Y[i]);	 
}

template<class T>
T CSpliner2D<T>::Spline(T xi1, T xi2)
{
	int n = x1.dim();
	for(int i=0; i<n; i++) y1[i] = Spliners1D[i]->Spline(xi2);
	CSpliner<T> S(x1,y1);
	return S(xi1);
}

template<class T>
CSpliner2D<T>::~CSpliner2D()
{
	int n = x1.dim();
	for(int i=0; i<n; i++) delete Spliners1D[i];
}


template class CSpliner2D<float>;
template class CSpliner2D<double>;


