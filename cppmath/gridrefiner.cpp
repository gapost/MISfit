#include "cppmath.h"

void invM(int n, CArray2D& W)
// Calculates the inverse of the 2nd derivative matrix
{
	CTridiagSym2D M(n);
	W = CArray2D(n,n);
	CArray1D b(n);

	int i,j;

	for(i=0; i<n; i++) {
		// Prepare Tridiag
		for(j=0; j<n; j++) { 
			M.d(j) = 2.; 
			if (j) M.s(j)=-1.;
			if (j==i) b[j] = 1.; else b[j] = 0.;
		}
		// Solve
		Dptsl(M,b);
		for(j=0; j<n; j++) W[j][i] = b[j];
	}	
}

CGridRefiner::CGridRefiner(int an) : 
n(an), n2(an-2), f(an-2), df(an-2), dx(an-2), x1(an-2), u(an-2),
newx(an), oldx(an), F(an) 
{ invM(an-2,W); }

void CGridRefiner::Init(int an) 
{ 
	n = an;
	n2 = n-2;
	assert(n2>0);
	f = CArray1D(n2);
	df = CArray1D(n2);
	dx = CArray1D(n2);
	x1 = CArray1D(n2);
	u = CArray1D(n2);
	newx = CArray1D(n);
	oldx = CArray1D(n);
	F = CArray1D(n); 
	invM(n2,W); 
}

void CGridRefiner::CalcF_(double* x, double* y)
{
/*	for(int i = 0; i<n2; i++) {
		double f1 = fabs((y[i+1]-y[i])/(x[i+1]-x[i]));
		double f2 = fabs((y[i]-y[i-1])/(x[i]-x[i-1]));
		f[i] = 0.5*(f1 + f2);
		df[i] = f1 - f2;
		dx[i] = (x[i+1]-x[i-1])/2;
	}
	*/
	for(int i = 0; i<n2; i++) {
		double f1 = (y[i+1]-y[i])/(x[i+1]-x[i]);
		double f2 = (y[i]-y[i-1])/(x[i]-x[i-1]);
		f[i] = 0.5*(f1*f1 + f2*f2);
		df[i] = f1*f1 - f2*f2;
		dx[i] = (x[i+1]-x[i-1])/2;
	}

}

bool CGridRefiner::CalcX1_(double delta, double* x)
{
	int i,j;

	for(i = 0; i<n2; i++) u[i] = delta * df[i]/(1+delta*f[i]);

	double L = *(x+n2) - *(x-1);
	int nn = n2-1;
	for(i = 0; i<n2; i++) {
		x1[i] = 0.;
		for(j=0; j<n2; j++) {
			x1[i] += W[i][j]*u[j];
			if (j==nn) x1[i] += W[i][j]*L;
		}
	}

	bool ret = x1[0]>0;

	i = 1;
	while(ret && i<n2) { ret = (x1[i]>x1[i-1]); i++; }

	if (!ret) return ret;

	return (L>x1[n2-1]);
}

bool CGridRefiner::Refine(double* x, double* y, double alpha)
{
	CalcF_(x+1,y+1);

	double dgood = 0, dbad = 0.001;
	while (CalcX1_(dbad,x+1)) { dgood = dbad; dbad *= 2; }
	while (fabs(2*(dgood-dbad)/(dgood+dbad))>1e-6) {
		double dn = 0.5*(dgood+dbad);
		if (CalcX1_(dn,x+1)) dgood = dn;
		else dbad = dn;
	}

	if (dgood>0) {
		CalcX1_(dgood*alpha,x+1);
		for(int i=0; i<n; i++) {
			oldx[i] = x[i];
			if (i==0 || i==n-1) 
				newx[i] = oldx[i];
			else {
				newx[i] = x1[i-1] + *x;
				x[i] = newx[i];
			}
		}
		return true;
	}
	else return false;
}

bool CGridRefiner::Refine(CArray1D& x, CArray1D& y, double alpha)
{
	assert(x.dim()==y.dim());
	assert(x.dim()==n);
	return Refine(&x[0],&y[0],alpha);
}



void CGridRefiner::Convert(double* z)
{
	for(int i=0; i<n; i++) F[i] = z[i];
	CSpliner<double> S(oldx,F);
	for(i=1; i<n-1; i++) z[i] = S(newx[i]);
}

void CGridRefiner::Convert(CArray1D& z)
{
	assert(z.dim()==n);
	Convert(&z[0]);
}


