#include "cppmath.h"

#include <math.h>

//#define TINY 1.0e-20 //A small number.

template<class T, class Tbase>
int ludcmp(int n, TNT::Array2D<T>& a, TNT::Array1D<int>& indx, int& d,
		   TNT::Array1D<Tbase>& vv, T tiny) //vv stores the implicit scaling of each row.
/*
Given a matrix a[1..n][1..n], this routine replaces it by the LU decomposition of a rowwise
permutation of itself. a and n are input. a is output, arranged as in equation (2.3.14) above;
indx[1..n] is an output vector that records the row permutation e.ected by the partial
pivoting; d is output as ±1 depending on whether the number of row interchanges was even
or odd, respectively. This routine is used in combination with lubksb to solve linear equations
or invert a matrix.
*/
{
	int i,imax,j,k;
	T sum, dum1;
	Tbase dum, big, temp;

	//TNT::Fortran_Array1D<Tbase> vv(n); //vv stores the implicit scaling of each row.

	d = 1; //No row interchanges yet.
	for (i=1;i<=n;i++) { //Loop over rows to get the implicit scaling information.
		big=0;
		for (j=1;j<=n;j++) 
			if ((temp = fabs(a(i,j))) > big) big=temp;
		if (big == 0.0) return CPPMATH_ERR_LU_SING; // No nonzero largest element.
		vv(i)=1.0/big; //Save the scaling.
	}
	for (j=1;j<=n;j++) { //This is the loop over columns of Crout’s method.
		for (i=1;i<j;i++) { //This is equation (2.3.12) except for i = j.
			sum=a(i,j);
			for (k=1;k<i;k++) sum -= a(i,k)*a(k,j);
			a(i,j)=sum;
		}
		big=0.0; //Initialize for the search for largest pivot element.
		for (i=j;i<=n;i++) { //This is i = j of equation (2.3.12) and i = j+1. . .N of equation (2.3.13). 
			sum=a(i,j);
			for (k=1;k<j;k++)
				sum -= a(i,k)*a(k,j);
			a(i,j)=sum;
			if ( (dum=vv(i)*fabs(sum)) >= big) {
				//Is the figure of merit for the pivot better than the best so far?
				big=dum;
				imax=i;
			}
		}
		if (j != imax) { //Do we need to interchange rows?
			for (k=1;k<=n;k++) { //Yes, do so...
				dum1=a(imax,k);
				a(imax,k)=a(j,k);
				a(j,k)=dum1;
			}
			d = -d; //...and change the parity of d.
			vv(imax)=vv(j); //Also interchange the scale factor.
		}
		indx(j)=imax;
		if (fabs(a(j,j)) == 0.0) {
			a(j,j)=tiny;
		}
		//If the pivot element is zero the matrix is singular (at least to the precision of the
		//algorithm). For some applications on singular matrices, it is desirable to substitute
		//TINY for zero.
		if (j != n) { // Now, finally, divide by the pivot element.
			dum1=(Tbase)(1.0)/(a(j,j));
			for (i=j+1;i<=n;i++) a(i,j) *= dum1;
		}
	} //Go back for the next column in the reduction.
	
	return CPPMATH_OK;
}

template<class T>
void lubksb(int n, const TNT::Array2D<T>& a, const TNT::Array1D<int>& indx, TNT::Array1D<T>& b)
/*
Solves the set of n linear equations A·X = B. Here a[1..n][1..n] is input, not as the matrix
A but rather as its LU decomposition, determined by the routine ludcmp. indx[1..n] is input
as the permutation vector returned by ludcmp. b[1..n] is input as the right-hand side vector
B, and returns with the solution vector X. a, n, and indx are not modi.ed by this routine
and can be left in place for successive calls with di.erent right-hand sides b. This routine takes
into account the possibility that b will begin with many zero elements, so it is e.cient for use
in matrix inversion.
*/
{
	int i,ii=0,ip,j;
	T sum;
	for (i=1;i<=n;i++) { //When ii is set to a positive value, it will become the
		//index of the first nonvanishing element of b. Wenow
		//do the forward substitution, equation (2.3.6). The
		//only new wrinkle is to unscramble the permutation
		//as we go.
		ip=indx(i);
		sum=b(ip);
		b(ip)=b(i);
		if (ii) for (j=ii;j<=i-1;j++) sum -= a(i,j)*b(j);
		else if (fabs(sum)) ii=i; //A nonzero element was encountered, so from now on we
		//will have to do the sums in the loop above. 
		b(i)=sum;
	}
	for (i=n;i>=1;i--) { //Now we do the backsubstitution, equation (2.3.7).
		sum=b(i);
		for (j=i+1;j<=n;j++) sum -= a(i,j)*b(j);
		b(i)=sum/a(i,i); //Store a component of the solution vector X.
	} //All done!
}

int Ludcmp(TNT::Array2D<float>& A, CIntArray1D& indx, int& d)
{
	int n = A.dim1();
	assert(A.dim2()==n);
	assert(indx.dim()==n);

	TNT::Array1D<float> vv(n);
	
	return ludcmp(n, A,	indx, d, vv, 1e-20f);
}

int Ludcmp(TNT::Array2D<cfloat>& A, CIntArray1D& indx, int& d)
{
	int n = A.dim1();
	assert(A.dim2()==n);
	assert(indx.dim()==n);

	TNT::Array1D<float> vv(n);
	
	return ludcmp(n, A,	indx, d, vv, cfloat(1e-20f,1e-20f));
}

int Ludcmp(CArray2D& A, CIntArray1D& indx, int& d)
{
	int n = A.dim1();
	assert(A.dim2()==n);
	assert(indx.dim()==n);

	TNT::Array1D<double> vv(n);
	
	return ludcmp(n, A,	indx, d, vv, 1e-30);
}

void Lubksb(TNT::Array2D<float>& A, CIntArray1D& indx, TNT::Array1D<float>& b)
{
	int n = A.dim1();
	assert(A.dim2()==n);
	assert(indx.dim()==n);

	lubksb(n, A, indx, b);
}

void Lubksb(TNT::Array2D<cfloat>& A, CIntArray1D& indx, TNT::Array1D<cfloat>& b)
{
	int n = A.dim1();
	assert(A.dim2()==n);
	assert(indx.dim()==n);

	lubksb(n, A, indx, b);
}

void Lubksb(CArray2D& A, CIntArray1D& indx, CArray1D& b)
{
	int n = A.dim1();
	assert(A.dim2()==n);
	assert(indx.dim()==n);

	lubksb(n, A, indx, b);
}

int Ludcmp(CComplexArray2D& A, CIntArray1D& indx, int& d)
{
	int n = A.dim1();
	assert(A.dim2()==n);
	assert(indx.dim()==n);
	
	TNT::Array1D<double> vv(n);

	return ludcmp(n, A,	indx, d, vv, cdouble(1e-30,1e-30));
}

void Lubksb(CComplexArray2D& A, CIntArray1D& indx, CComplexArray1D& b)
{
	int n = A.dim1();
	assert(A.dim2()==n);
	assert(indx.dim()==n);
	
	lubksb(n, A, indx, b);
}


template <class T>
int dgtsl(int n,							// order
		  TNT::Fortran_Array1D<T>& c,	// subdiag
		  TNT::Fortran_Array1D<T>& d,	// diagonal
  		  TNT::Fortran_Array1D<T>& e,	// superdiag
		  TNT::Fortran_Array1D<T>& b)	// right hand
/*
      subroutine dgtsl(n,c,d,e,b,info)
      integer n,info
      double precision c(1),d(1),e(1),b(1)
c
c     dgtsl given a general tridiagonal matrix and a right hand
c     side will find the solution.
c
c     on entry
c
c        n       integer
c                is the order of the tridiagonal matrix.
c
c        c       double precision(n)
c                is the subdiagonal of the tridiagonal matrix.
c                c(2) through c(n) should contain the subdiagonal.
c                on output c is destroyed.
c
c        d       double precision(n)
c                is the diagonal of the tridiagonal matrix.
c                on output d is destroyed.
c
c        e       double precision(n)
c                is the superdiagonal of the tridiagonal matrix.
c                e(1) through e(n-1) should contain the superdiagonal.
c                on output e is destroyed.
c
c        b       double precision(n)
c                is the right hand side vector.
c
c     on return
c
c        b       is the solution vector.
c
c        info    integer
c                = 0 normal value.
c                = k if the k-th element of the diagonal becomes
c                    exactly zero.  the subroutine returns when
c                    this is detected.
c
c     linpack. this version dated 08/14/78 .
c     jack dongarra, argonne national laboratory.
c
c     no externals
c     fortran dabs
c
c     internal variables
c
      integer k,kb,kp1,nm1,nm2
      double precision t
c     begin block permitting ...exits to 100
c*/
{
	int k,kb,kp1,nm1,nm2;
	T  t;
	
	int info = 0;
	c(1) = d(1);
	nm1 = n - 1;
	if (nm1 >= 1) { //goto L40;
		d(1) = e(1);
		e(1) = 0.0;
		e(n) = 0.0;
		
		for(k=1; k<=nm1; k++) { //do 30 k = 1, nm1
			kp1 = k + 1;
			
			//find the largest of the two rows
			
			if (!(fabs(c(kp1)) < fabs(c(k)))) { //goto L10;
				// interchange row
				t = c(kp1);
				c(kp1) = c(k);
				c(k) = t;
				t = d(kp1);
				d(kp1) = d(k);
				d(k) = t;
				t = e(kp1);
				e(kp1) = e(k);
				e(k) = t;
				t = b(kp1);
				b(kp1) = b(k);
				b(k) = t;
			} /*L10:*///continue;
			
			// zero elements
			
			if (c(k) == (T)(0.0)) { //goto 20
				info = k;
				// exit
				return info;
			}/*20*/ //  continue
			t = -c(kp1)/c(k);
			c(kp1) = d(kp1) + t*d(k);
			d(kp1) = e(kp1) + t*e(k);
			e(kp1) = 0.0;
			b(kp1) = b(kp1) + t*b(k);
		} //do 30 k = 1, nm1

	} //L40:    continue;

	if (c(n) == (T)(0.0)) { //goto 50
		info = n;
		return info;
	} //50    continue
	
	// back solve
	
	nm2 = n - 2;
	b(n) = b(n)/c(n);
	if (n>1) { //goto L80;
		b(nm1) = (b(nm1) - d(nm1)*b(n))/c(nm1);
		if (nm2 >= 1) { //goto L70;
			for(kb=1; kb<=nm2; kb++) { //do 60 kb = 1, nm2
				k = nm2 - kb + 1;
				b(k) = (b(k) - d(k)*b(k+1) - e(k)*b(k+2))/c(k);
			} //60          continue
		} //70          continue
	} // 80       continue
	return info;
}

int Dgtsl(Tridiag_Array2D<float>& T, TNT::Array1D<float>& b)	
/* solve tridiagonal linear system 
		T x X = b

	T : tridiag matrix. destroyed on output
	b : right hand. on output contains the solution 
*/
{
	int n = T.dim();
#ifdef _DEBUG
	assert(b.dim()==n);
#endif

	return dgtsl(n,							// order
				 TNT::Fortran_Array1D<float>(n,T.gets()),	// subdiag
				 TNT::Fortran_Array1D<float>(n,T.getd()),	// diagonal
  				 TNT::Fortran_Array1D<float>(n,T.gete()),	// superdiag
				 TNT::Fortran_Array1D<float>(b));
}

int Dgtsl(Tridiag_Array2D<cfloat>& T, TNT::Array1D<cfloat>& b)	
/* solve tridiagonal linear system 
		T x X = b

	T : tridiag matrix. destroyed on output
	b : right hand. on output contains the solution 
*/
{
	int n = T.dim();
#ifdef _DEBUG
	assert(b.dim()==n);
#endif

	return dgtsl(n,							// order
				 TNT::Fortran_Array1D<cfloat>(n,T.gets()),	// subdiag
				 TNT::Fortran_Array1D<cfloat>(n,T.getd()),	// diagonal
  				 TNT::Fortran_Array1D<cfloat>(n,T.gete()),	// superdiag
				 TNT::Fortran_Array1D<cfloat>(b));
}

int Dgtsl(CTridiag2D& T, CArray1D& b)	
/* solve tridiagonal linear system 
		T x X = b

	T : tridiag matrix. destroyed on output
	b : right hand. on output contains the solution 
*/
{
	int n = T.dim();
#ifdef _DEBUG
	assert(b.dim()==n);
#endif

	return dgtsl(n,							// order
				 TNT::Fortran_Array1D<double>(n,T.gets()),	// subdiag
				 TNT::Fortran_Array1D<double>(n,T.getd()),	// diagonal
  				 TNT::Fortran_Array1D<double>(n,T.gete()),	// superdiag
				 TNT::Fortran_Array1D<double>(b));
}

int Dgtsl(CComplexTridiag2D& T, CComplexArray1D& b)	
/* solve tridiagonal linear system 
		T x X = b

	T : tridiag matrix. destroyed on output
	b : right hand. on output contains the solution 
*/
{
	int n = T.dim();
#ifdef _DEBUG
	assert(b.dim()==n);
#endif

	return dgtsl(n,							// order
				 TNT::Fortran_Array1D<cdouble>(n,T.gets()),	// subdiag
				 TNT::Fortran_Array1D<cdouble>(n,T.getd()),	// diagonal
  				 TNT::Fortran_Array1D<cdouble>(n,T.gete()),	// superdiag
				 TNT::Fortran_Array1D<cdouble>(b));
}

void dptsl(int n, 
		   TNT::Fortran_Array1D<double>& d,
		   TNT::Fortran_Array1D<double>& e,
		   TNT::Fortran_Array1D<double>& b)
/*c
c     dptsl given a positive definite tridiagonal matrix and a right
c     hand side will find the solution.
c
c     on entry
c
c        n        integer
c                 is the order of the tridiagonal matrix.
c
c        d        double precision(n)
c                 is the diagonal of the tridiagonal matrix.
c                 on output d is destroyed.
c
c        e        double precision(n)
c                 is the offdiagonal of the tridiagonal matrix.
c                 e(1) through e(n-1) should contain the
c                 offdiagonal.
c
c        b        double precision(n)
c                 is the right hand side vector.
c
c     on return
c
c        b        contains the soultion.
c
c     linpack. this version dated 08/14/78 .
c     jack dongarra, argonne national laboratory.
c
c     no externals
c     fortran mod
c
c     internal variables
c*/
{
	int k,kbm1,ke,kf,kp1,nm1,nm1d2;
	double t1,t2;
	//
	//     check for 1 x 1 case
	//
	if (n == 1) {
		b(1) = b(1)/d(1);
		return;
	}
	nm1 = n - 1;
	nm1d2 = nm1/2;
	if (n != 2) { //go to 30
		kbm1 = n - 1;
		//
		//           zero top half of subdiagonal and bottom half of
		//           superdiagonal
		//
		for(k = 1; k<=nm1d2; k++) {
			t1 = e(k)/d(k);
			d(k+1) = d(k+1) - t1*e(k);
			b(k+1) = b(k+1) - t1*b(k);
			t2 = e(kbm1)/d(kbm1+1);
			d(kbm1) = d(kbm1) - t2*e(kbm1);
			b(kbm1) = b(kbm1) - t2*b(kbm1+1);
			kbm1 = kbm1 - 1;
		} //20       continue
	} // 30    continue
	kp1 = nm1d2 + 1;
	//
	//        clean up for possible 2 x 2 block at center
	//
	if ( (n%2)==0 ) { //if (mod(n,2) .ne. 0) go to 40
		t1 = e(kp1)/d(kp1);
		d(kp1+1) = d(kp1+1) - t1*e(kp1);
		b(kp1+1) = b(kp1+1) - t1*b(kp1);
		kp1 = kp1 + 1;
	} //40    continue
	//
	//        back solve starting at the center, going towards the top
	//        and bottom
	//
	b(kp1) = b(kp1)/d(kp1);
	if (n!=2) { //if (n .eq. 2) go to 60
		k = kp1 - 1;
		ke = kp1 + nm1d2 - 1;
		for(kf=kp1; kf<=ke; kf++) { // do 50 kf = kp1, ke
			b(k) = (b(k) - e(k)*b(k+1))/d(k);
			b(kf+1) = (b(kf+1) - e(kf)*b(kf))/d(kf+1);
			k = k - 1;
		} // 50       continue
	} //60    continue
	if ((n%2)==0) b(1) = (b(1) - e(1)*b(2))/d(1);
	//70 continue
}


void Dptsl(CTridiagSym2D& T, CArray1D& b)	
/* solve the linear system
 
		T x X = b

  where T is positive definite tridiagonal

	T : tridiag matrix. diagonal destroyed on output
	b : right hand. on output contains the solution 
*/
{
	int n = T.dim();
#ifdef _DEBUG
	assert(b.dim()==n);
#endif
	dptsl(n,							// order
		TNT::Fortran_Array1D<double>(n,T.getd()),	// diagonal
		TNT::Fortran_Array1D<double>(n-1,T.gets()+1),	// subdiag
		TNT::Fortran_Array1D<double>(b));
}