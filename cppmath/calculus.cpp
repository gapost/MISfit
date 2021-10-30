#include "cppmath.h"
#include <math.h>

CArray1D savgol(int nl, int nr, int ld, int m)
/*
Returns in c[1..np], in wrap-around order (N.B.!) consistent with the argument respns in
routine convlv, a set of Savitzky-Golay .lter coe.cients. nl is the number of leftward (past)
data points used, while nr is the number of rightward (future) data points, making the total
number of data points used nl+nr+1. ld is the order of the derivative desired (e.g., ld = 0
for smoothed function). m is the order of the smoothing polynomial, also equal to the highest
conserved moment; usual values are m = 2 or m = 4.
*/
{
	int imj,ipj,j,k,kk,mm;
	double fac,sum;
	int d;
	int np = nl+nr+1;
	if (nl < 0 || nr < 0 || ld > m || nl+nr < m) return CArray1D();
	
	TNT::Array1D<int> c_indx(m+1);
	TNT::Array2D<double> c_a(m+1,m+1);
	TNT::Array1D<double> c_b(m+1);
	TNT::Array1D<double> c_c(np);
	TNT::Fortran_Array1D<int> indx(c_indx);
	TNT::Fortran_Array2D<double> a(c_a);
	TNT::Fortran_Array1D<double> b(c_b);
	TNT::Fortran_Array1D<double> c(c_c);

	
	for (ipj=0;ipj<=(m << 1);ipj++) { //Set up the normal equations of the desired least-squares fit 
		sum=(ipj ? 0.0 : 1.0);
		for (k=1;k<=nr;k++) sum += pow((double)k,(double)ipj);
		for (k=1;k<=nl;k++) sum += pow((double)-k,(double)ipj);
		mm=min(ipj,2*m-ipj);
		for (imj = -mm;imj<=mm;imj+=2) a(1+(ipj+imj)/2,1+(ipj-imj)/2)=sum;
	}
	Ludcmp(c_a,c_indx,d); //Solve them: LU decomposition.
	for (j=1;j<=m+1;j++) b(j)=0.0;
	b(ld+1)=1.0;
	//Right-hand side vector is unit vector, depending on which derivative we want.
	Lubksb(c_a,c_indx,c_b); //Get one row of the inverse matrix.
	for (kk=1;kk<=np;kk++) c(kk)=0.0; //Zero the output array (it may be bigger than number of coefficients). 
	for (k = -nl;k<=nr;k++) {
		sum=b(1); //Each Savitzky-Golay coe.cient is the dot
		//product of powers of an integer with the
		//inverse matrix row.
		fac=1.0;
		for (mm=1;mm<=m;mm++) sum += b(mm+1)*(fac *= k);
		//kk=((np-k) % np)+1; //Store in wrap-around order.
		kk = k+nl+1;
		c(kk)=sum;
	}
	return c_c;
}

CArray1D savgolfilt(const CArray1D& x, int nlr,  int m, int ld, double dx)
/*
Filters signal x with Savitzky-Golay filter.
nlr: points left and right
m: polynomial order
ld: derivative (ld=1 first derivative etc.)
*/
{
	int n = x.dim(), npar = 2*nlr+1;
	
	if (nlr < 0 || n==0 || ld > m || 2*nlr < m) return CArray1D();
	
	CArray1D c;
	//if (c.dim()==0) return CArray1D();
	CArray1D fx(n,0.);

	int i,j,k;

	for(i=0; i<nlr; i++) {
		c = savgol(i,nlr,ld,m);
		for(j=0; j<=i+nlr; j++) 
			fx[i] += x[j]*c[j];
	}
	for(i=n-nlr; i<n; i++) {
		int nr = n-i-1;
		c = savgol(nlr,nr,ld,m);
		k = n-1;
		for(j=nr+nlr; j>=0; j--) 
			fx[i] += x[k--]*c[j];
	}

	c = savgol(nlr, nlr, ld, m);
	for(i=nlr; i<n-nlr; i++) {
		double* px = x.getdata() + i - nlr;
		double* pc = &(c[0]);
		for(j=0; j<npar; j++) {
			fx[i] += (*pc)*(*px);
			pc++;
			px++;
		}
	}

	if(ld>0 && dx!=1. && dx!=0.) {
		double dd = dx;
		ld--;
		while (ld>0) { dd *= dx; ld--; } 
		for(i=0; i<n; i++) fx[i] /= dd;
	}
	
	return fx;	
}