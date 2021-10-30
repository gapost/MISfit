#include "cppmath.h"

#include <math.h>

float pythag(float a, float b)
// Computes (a2 + b2)^1/2 without destructive under.ow or over.ow.
{
	float absa,absb;
	absa=fabs(a);
	absb=fabs(b);
	if (absa > absb) return absa*sqrt(1.0f+fsqr(absb/absa));
	else return (absb == 0.0 ? 0.0 : absb*sqrt(1.0f+fsqr(absa/absb)));
}
double pythag(double a, double b)
// Computes (a2 + b2)^1/2 without destructive under.ow or over.ow.
{
	double absa,absb;
	absa=fabs(a);
	absb=fabs(b);
	if (absa > absb) return absa*sqrt(1.0+fsqr(absb/absa));
	else return (absb == 0.0 ? 0.0 : absb*sqrt(1.0+fsqr(absa/absb)));
}
float dsign(float a, float b)
{
	float c= fabs(a);
	if (b>=0) return c;
	else return -c;
}
double dsign(double a, double b)
{
	double c= fabs(a);
	if (b>=0) return c;
	else return -c;
}

double epslon (double x)
/*c
c     estimate unit roundoff in quantities of size x.
c
      double precision a,b,c,eps
c
c     this program should function properly on all systems
c     satisfying the following two assumptions,
c        1.  the base used in representing floating point
c            numbers is not a power of three.
c        2.  the quantity  a  in statement 10 is represented to 
c            the accuracy used in floating point variables
c            that are stored in memory.
c     the statement number 10 and the go to 10 are intended to
c     force optimizing compilers to generate code satisfying 
c     assumption 2.
c     under these assumptions, it should be true that,
c            a  is not exactly equal to four-thirds,
c            b  has a zero for its last bit or digit,
c            c  is not exactly equal to one,
c            eps  measures the separation of 1.0 from
c                 the next larger floating point number.
c     the developers of eispack would appreciate being informed
c     about any systems where these assumptions do not hold.
c
c     this version dated 4/6/83.
c*/
{
	 double a = 4.0/3.0;
L10: double b = a - 1.0;
	 double c = b + b + b;
	 double eps = fabs(c-1.0);
	 if (eps == 0.0) goto L10;
	 return eps*fabs(x);
}
float epslon (float x)
/*c
c     estimate unit roundoff in quantities of size x.
c
      double precision a,b,c,eps
c
c     this program should function properly on all systems
c     satisfying the following two assumptions,
c        1.  the base used in representing floating point
c            numbers is not a power of three.
c        2.  the quantity  a  in statement 10 is represented to 
c            the accuracy used in floating point variables
c            that are stored in memory.
c     the statement number 10 and the go to 10 are intended to
c     force optimizing compilers to generate code satisfying 
c     assumption 2.
c     under these assumptions, it should be true that,
c            a  is not exactly equal to four-thirds,
c            b  has a zero for its last bit or digit,
c            c  is not exactly equal to one,
c            eps  measures the separation of 1.0 from
c                 the next larger floating point number.
c     the developers of eispack would appreciate being informed
c     about any systems where these assumptions do not hold.
c
c     this version dated 4/6/83.
c*/
{
	 float a = 4.0f/3.0f;
L10: float b = a - 1.0f;
	 float c = b + b + b;
	 float eps = fabs(c-1.0f);
	 if (eps == 0.0f) goto L10;
	 return eps*fabs(x);
}

int imtql2(int n, TNT::Fortran_Array1D<double>& d, 
		   TNT::Fortran_Array1D<double>& e,
		   TNT::Fortran_Array2D<double>& z)
/*
      integer i,j,k,l,m,n,ii,nm,mml,ierr
      double precision d(n),e(n),z(nm,n)
      double precision b,c,f,g,p,r,s,tst1,tst2,pythag
c
c     this subroutine is a translation of the algol procedure imtql2,
c     num. math. 12, 377-383(1968) by martin and wilkinson,
c     as modified in num. math. 15, 450(1970) by dubrulle.
c     handbook for auto. comp., vol.ii-linear algebra, 241-248(1971).
c
c     this subroutine finds the eigenvalues and eigenvectors
c     of a symmetric tridiagonal matrix by the implicit ql method.
c     the eigenvectors of a full symmetric matrix can also
c     be found if  tred2  has been used to reduce this
c     full matrix to tridiagonal form.
c
c     on input
c
c        nm must be set to the row dimension of two-dimensional
c          array parameters as declared in the calling program
c          dimension statement.
c
c        n is the order of the matrix.
c
c        d contains the diagonal elements of the input matrix.
c
c        e contains the subdiagonal elements of the input matrix
c          in its last n-1 positions.  e(1) is arbitrary.
c
c        z contains the transformation matrix produced in the
c          reduction by  tred2, if performed.  if the eigenvectors
c          of the tridiagonal matrix are desired, z must contain
c          the identity matrix.
c
c      on output
c
c        d contains the eigenvalues in ascending order.  if an
c          error exit is made, the eigenvalues are correct but
c          unordered for indices 1,2,...,ierr-1.
c
c        e has been destroyed.
c
c        z contains orthonormal eigenvectors of the symmetric
c          tridiagonal (or full) matrix.  if an error exit is made,
c          z contains the eigenvectors associated with the stored
c          eigenvalues.
c
c        ierr is set to
c          zero       for normal return,
c          j          if the j-th eigenvalue has not been
c                     determined after 30 iterations.
c
c     calls pythag for  dsqrt(a*a + b*b) .
c
c     questions and comments should be directed to burton s. garbow,
c     mathematics and computer science div, argonne national laboratory
c
c     this version dated august 1983.
c
c     ------------------------------------------------------------------
*/
{
	int i,j,k,l,m,ii,mml,ierr;
	double b,c,f,g,p,r,s,tst1,tst2;
	
	ierr = 0;
	if (n == 1) goto L1001;
	//
	for(i = 2; i<=n; i++) e(i-1) = e(i);
	//
	e(n) = 0.0;
	//
	for(l = 1; l<=n; l++) {
		j = 0;
		//     .......... look for small sub-diagonal element ..........
L105:    for(m = l; m<=n; m++) {
			 if (m == n) goto L120;
			 tst1 = fabs(d(m)) + fabs(d(m+1));
			 tst2 = tst1 + fabs(e(m));
			 if (tst2 == tst1) goto L120;
		 } // m=l..n
		 
L120:    p = d(l);
         if (m == l) continue; // the m=l..n loop //goto L240;
         if (j == 30) goto L1000;
         j = j + 1;
		 //     .......... form shift ..........
         g = (d(l+1) - p) / (2.0 * e(l));
         r = pythag(g,1.0);
         g = d(m) - p + e(l) / (g + dsign(r,g));
         s = 1.0;
         c = 1.0;
         p = 0.0;
         mml = m - l;
		 //     .......... for i=m-1 step -1 until l do -- ..........
         for(ii = 1; ii<=mml; ii++) {
			 i = m - ii;
			 f = s * e(i);
			 b = c * e(i);
			 r = pythag(f,g);
			 e(i+1) = r;
			 if (r == 0.0) goto L210;
			 s = f / r;
			 c = g / r;
			 g = d(i+1) - p;
			 r = (d(i) - g) * s + 2.0 * c * b;
			 p = s * r;
			 d(i+1) = g + p;
			 g = c * r - b;
			 //     .......... form vector ..........
			 for(k = 1; k<=n; k++) {
				 f = z(k,i+1);
				 z(k,i+1) = s * z(k,i) + c * f;
				 z(k,i) = c * z(k,i) - s * f;
			 } // k=1..n
			 //
		 } // ii=1..mml
		 //
         d(l) = d(l) - p;
         e(l) = g;
         e(m) = 0.0;
         goto L105;
		 //     .......... recover from underflow ..........
L210:    d(i+1) = d(i+1) - p;
         e(m) = 0.0;
         goto L105;
	} // l=1..n L240
	//     .......... order eigenvalues and eigenve//tors ..........
	for(ii = 2; ii<=n; ii++) {
		i = ii - 1;
		k = i;
		p = d(i);
		//
		for(j = ii; j<=n; j++) {
			if (d(j) >= p) continue; //goto L260;
			k = j;
			p = d(j);
		}  /*L260:*/
		//
		if (k == i) continue; //goto L300;
		d(k) = d(i);
		d(i) = p;
		//
		for(j = 1; j<=n; j++) {
			p = z(j,i);
			z(j,i) = z(j,k);
			z(j,k) = p;
		}  /*L280:*/ 
		
	}  // L300: ii = 2..n
	
	goto L1001;
	//     .......... set error -- no convergence to an
	//                eigenvalue after 30 iterations ..........
L1000: ierr = l;
L1001: return ierr;
}

int Imtql2(const CTridiagSym2D& T, CArray1D& W, CArray2D& Z, CArray1D& g)
/*
Computes eigenvectors & eigenfunctions of symmetric tridiagonal matrices

  W[k] : eigenvalues (ordered).
  Z[k][n] : input the identity matrix; on output contains the eigenvectors. k numbers the eigenvalue.
  z[k][i]*A[i][j]*z[k][j] = l[k]; A: matrix, l: eigenvalues
  g : garbage

  return value:
	0 : converged
	j : did not converge at jth eigenvalue


*/
{
	int n = T.dim();
#ifdef _DEBUG
	assert(n>1);
	assert(W.dim()==n);
	assert(Z.dim1()==n);
	assert(g.dim()==n);
#endif

	W[0] = T.d(0);
	for(int i=1; i<n; i++) {
		W[i] = T.d(i);
		g[i] = T.s(i);
	}
	
	return imtql2(n,
		TNT::Fortran_Array1D<double>(W),
		TNT::Fortran_Array1D<double>(g),
		TNT::Fortran_Array2D<double>(Z)
		);
}

template<class T>
int tsturm(int n, T eps1,
		   TNT::Fortran_Array1D<T>& d,
		   TNT::Fortran_Array1D<T>& e,
		   T lb, T ub, int mm, int& m,
		   TNT::Fortran_Array1D<T>& w,
		   TNT::Fortran_Array2D<T>& z,
		   TNT::Array2D<T>& g)
/*
c     this subroutine is a translation of the algol procedure tristurm
c     by peters and wilkinson.
c     handbook for auto. comp., vol.ii-linear algebra, 418-439(1971).
c
c     this subroutine finds those eigenvalues of a tridiagonal
c     symmetric matrix which lie in a specified interval and their
c     associated eigenvectors, using bisection and inverse iteration.
c
c     on input
c
c        nm must be set to the row dimension of two-dimensional
c          array parameters as declared in the calling program
c          dimension statement.
c
c        n is the order of the matrix.
c
c        eps1 is an absolute error tolerance for the computed
c          eigenvalues.  it should be chosen commensurate with
c          relative perturbations in the matrix elements of the
c          order of the relative machine precision.  if the
c          input eps1 is non-positive, it is reset for each
c          submatrix to a default value, namely, minus the
c          product of the relative machine precision and the
c          1-norm of the submatrix.
c
c        d contains the diagonal elements of the input matrix.
c
c        e contains the subdiagonal elements of the input matrix
c          in its last n-1 positions.  e(1) is arbitrary.
c
c        e2 contains the squares of the corresponding elements of e.
c          e2(1) is arbitrary.
c
c        lb and ub define the interval to be searched for eigenvalues.
c          if lb is not less than ub, no eigenvalues will be found.
c
c        mm should be set to an upper bound for the number of
c          eigenvalues in the interval.  warning. if more than
c          mm eigenvalues are determined to lie in the interval,
c          an error return is made with no values or vectors found.
c
c     on output
c
c        eps1 is unaltered unless it has been reset to its
c          (last) default value.
c
c        d and e are unaltered.
c
c        elements of e2, corresponding to elements of e regarded
c          as negligible, have been replaced by zero causing the
c          matrix to split into a direct sum of submatrices.
c          e2(1) is also set to zero.
c
c        m is the number of eigenvalues determined to lie in (lb,ub).
c
c        w contains the m eigenvalues in ascending order if the matrix
c          does not split.  if the matrix splits, the eigenvalues are
c          in ascending order for each submatrix.  if a vector error
c          exit is made, w contains those values already found.
c
c        z contains the associated set of orthonormal eigenvectors.
c          if an error exit is made, z contains those vectors
c          already found.
c
c        ierr is set to
c          zero       for normal return,
c          3*n+1      if m exceeds mm.
c          4*n+r      if the eigenvector corresponding to the r-th
c                     eigenvalue fails to converge in 5 iterations.
c
c        rv1, rv2, rv3, rv4, rv5, and rv6 are temporary storage arrays.
c
c     the algol procedure sturmcnt contained in tristurm
c     appears in tsturm in-line.
c
c     calls pythag for  dsqrt(a*a + b*b) .
c
c     questions and comments should be directed to burton s. garbow,
c     mathematics and computer science div, argonne national laboratory
c
c     this version dated august 1983.
c
c     ------------------------------------------------------------------
*/
//		   e2,lb,ub,mm,m,w,z,                  ierr,rv1,rv2,rv3,rv4,rv5,rv6)
{
/*	ofstream foo("tsturm.dat");
	foo << "n = " << n << endl;
	foo << "eps1 = " << eps1 << endl;
	foo << " diagonal " << endl << d;
	foo << " subdiagonal " << endl << e;
	foo << "lbound = " << lb << endl;
	foo << "ubound = " << ub << endl;
	foo << "mm = " << mm << endl;
	foo << "m = " << m << endl;
	foo << " w " << endl << w;
	foo << " z " << endl << z;
	foo << " g " << endl << g;
	foo.close();*/

	int i,j,k,p,q,r,s,ii,ip,jj,m1,m2,its,ierr,group,isturm;
	  TNT::Fortran_Array1D<T> e2(n,g[0]),
		  rv1(n,g[1]),
		  rv2(n,g[2]),
		  rv3(n,g[3]),
		  rv4(n,g[4]),
		  rv5(n,g[5]),
		  rv6(n,g[6]);
      T u,v,t1,t2,uk,xu,x0,x1,eps2,eps3,eps4,
            norm,tst1,tst2;

	  for(i=2; i<=n; i++) e2(i) = e(i)*e(i);

      ierr = 0;
      t1 = lb;
      t2 = ub;
//     .......... look for small sub-diagonal entries ..........
      for(i = 1; i<=n; i++) {
         if (i == 1) goto L20;
         tst1 = fabs(d(i)) + fabs(d(i-1));
         tst2 = tst1 + fabs(e(i));
         if (tst2 > tst1) continue;
L20:    e2(i) = 0.0;
	  }
//     .......... determine the number of eigenvalues
//                in the interval ..........
      p = 1;
      q = n;
      x1 = ub;
      isturm = 1;
      goto L320;
L60: m = s;
      x1 = lb;
      isturm = 2;
      goto L320;
L80: m = m - s;
      if (m > mm) goto L980;
      q = 0;
      r = 0;
//     .......... establish and process next submatrix, refining
//                interval by the gerschgorin bounds ..........
L100: if (r == m) goto L1001;
      p = q + 1;
      xu = d(p);
      x0 = d(p);
      u = 0.0;

      for(q = p; q<=n; q++) {
         x1 = u;
         u = 0.0;
         v = 0.0;
         if (q == n) goto L110;
         u = fabs(e(q+1));
         v = e2(q+1);
L110:    xu = min(d(q)-(x1+u),xu);
         x0 = max(d(q)+(x1+u),x0);
         if (v == 0.0) goto L140;
	  }

L140: x1 = epslon(max(fabs(xu),fabs(x0)));
      if (eps1 <= 0.0) eps1 = -x1;
      if (p != q) goto L180;
//     .......... check for isolated root within interval ..........
      if (t1 > d(p) || d(p) >= t2) goto L940;
      r = r + 1;

      for(i = 1; i<=n; i++) z(i,r) = 0.0;

      w(r) = d(p);
      z(p,r) = 1.0;
      goto L940;
L180: u = q-p+1;
      x1 = u * x1;
      lb = max(t1,xu-x1);
      ub = min(t2,x0+x1);
      x1 = lb;
      isturm = 3;
      goto L320;
L200: m1 = s + 1;
      x1 = ub;
      isturm = 4;
      goto L320;
L220: m2 = s;
      if (m1 > m2) goto L940;
//     .......... find roots by bisection ..........
      x0 = ub;
      isturm = 5;

      for(i = m1; i <= m2; i++) {
         rv5(i) = ub;
         rv4(i) = lb;
	  }
//     .......... loop for k-th eigenvalue
//                for k=m2 step -1 until m1 do --
//                (-do- not used to legalize -computed goto-) ..........
      k = m2;
L250:    xu = lb;
//     .......... for i=k step -1 until m1 do -- ..........
         for(ii = m1; ii<=k; ii++) {
            i = m1 + k - ii;
            if (xu >= rv4(i)) continue;
            xu = rv4(i);
            goto L280;
		 }

L280:    if (x0 > rv5(k)) x0 = rv5(k);
//     .......... next bisection step ..........
L300:    x1 = (xu + x0) * 0.5;
         if ((x0 - xu) <= fabs(eps1)) goto L420;
         tst1 = 2.0 * (fabs(xu) + fabs(x0));
         tst2 = tst1 + (x0 - xu);
         if (tst2 == tst1) goto L420;
//     .......... in-line procedure for sturm sequence ..........
L320:    s = p - 1;
         u = 1.0;

         for(i = p; i<=q; i++) {
            if (u != 0.0) goto L325;
            v = fabs(e(i)) / epslon(1.0);
            if (e2(i) == 0.0) v = 0.0;
            goto L330;
L325:       v = e2(i) / u;
L330:       u = d(i) - x1 - v;
            if (u < 0.0) s = s + 1;
		 }

         //goto (60,80,200,220,360), isturm
		 if (isturm>0 && isturm<6) {
			 switch (isturm)
			 {
			 case 1: goto L60; break;
			 case 2: goto L80; break;
			 case 3: goto L200; break;
			 case 4: goto L220; break;
			 case 5: goto L360; break;
			 }
		 }
//     .......... refine intervals ..........
L360:    if (s >= k) goto L400;
         xu = x1;
         if (s >= m1) goto L380;
         rv4(m1) = x1;
         goto L300;
L380:    rv4(s+1) = x1;
         if (rv5(s) > x1) rv5(s) = x1;
         goto L300;
L400:    x0 = x1;
         goto L300;
//     .......... k-th eigenvalue found ..........
L420:    rv5(k) = x1;
      k = k - 1;
      if (k >= m1) goto L250;
//     .......... find vectors by inverse iteration ..........
      norm = fabs(d(p));
      ip = p + 1;

      for(i = ip; i<=q; i++ )
			norm = max(norm, (T)(fabs(d(i)) + fabs(e(i))));
//     .......... eps2 is the criterion for grouping,
//                eps3 replaces zero pivots and equal
//                roots are modified by eps3,
//                eps4 is taken very small to avoid overflow ..........
      eps2 = 1.0-3 * norm;
      eps3 = epslon(norm);
      uk = q - p + 1;
      eps4 = uk * eps3;
      uk = eps4 / sqrt(uk);
      group = 0;
      s = p;

      for(k = m1; k <= m2; k++) {
         r = r + 1;
         its = 1;
         w(r) = rv5(k);
         x1 = rv5(k);
//     .......... look for close or coincident roots ..........
         if (k == m1) goto L520;
         if (x1 - x0 >= eps2) group = -1;
         group = group + 1;
         if (x1 <= x0) x1 = x0 + eps3;
//     .......... elimination with interchanges and
//                initialization of vector ..........
L520:    v = 0.0;

         for(i=p; i<=q; i++) { //do 580 i = p, q
            rv6(i) = uk;
            if (i == p) goto L560;
            if (fabs(e(i)) < fabs(u)) goto L540;
            xu = u / e(i);
            rv4(i) = xu;
            rv1(i-1) = e(i);
            rv2(i-1) = d(i) - x1;
            rv3(i-1) = 0.0;
            if (i != q) rv3(i-1) = e(i+1);
            u = v - xu * rv2(i-1);
            v = -xu * rv3(i-1);
            continue;
L540:       xu = e(i) / u;
            rv4(i) = xu;
            rv1(i-1) = u;
            rv2(i-1) = v;
            rv3(i-1) = 0.0;
L560:       u = d(i) - x1 - xu * v;
            if (i != q) v = e(i+1);
		 } // i=p..q 580    continue

         if (u == 0.0) u = eps3;
         rv1(q) = u;
         rv2(q) = 0.0;
         rv3(q) = 0.0;
//     .......... back substitution
//                for i=q step -1 until p do -- ..........
L600:    for(ii = p; ii <= q; ii++) {
            i = p + q - ii;
            rv6(i) = (rv6(i) - u * rv2(i) - v * rv3(i)) / rv1(i);
            v = u;
            u = rv6(i);
		 }
//     .......... orthogonalize with respect to previous
//                members of group ..........
         if (group == 0) goto L700;

         for(jj=1; jj<=group; jj++) { //do 680 jj = 1, group
            j = r - group - 1 + jj;
            xu = 0.0;

            for(i=p; i<=q; i++) xu = xu + rv6(i) * z(i,j);

            for(i=p; i<=q; i++) rv6(i) = rv6(i) - xu * z(i,j);

		 }

L700:    norm = 0.0;

         for(i=p; i<=q; i++) norm = norm + fabs(rv6(i));

         if (norm >= 1.0) goto L840;
//     .......... forward substitution ..........
         if (its == 5) goto L960;
         if (norm != 0.0) goto L740;
         rv6(s) = eps4;
         s = s + 1;
         if (s > q) s = p;
         goto L780;
L740:    xu = eps4 / norm;

         for(i=p; i<=q; i++) rv6(i) = rv6(i) * xu;
//     .......... elimination operations on next vector
//                iterate ..........
L780:    for(i=ip; i<=q; i++) { //do 820 i = ip, q
            u = rv6(i);
//     .......... if rv1(i-1) == e(i), a row interchange
//                was performed earlier in the
//                triangularization process ..........
            if (rv1(i-1) != e(i)) goto L800;
            u = rv6(i-1);
            rv6(i-1) = rv6(i);
L800:       rv6(i) = u - rv4(i) * rv6(i-1);
		 }

         its = its + 1;
         goto L600;
//     .......... normalize so that sum of squares is
//               1 and expand to full order ..........
L840:    u = 0.0;

         for(i=p; i<=q; i++) u = pythag(u,rv6(i));

         xu = 1.0 / u;

         for(i=1; i<=n; i++) z(i,r) = 0.0;

         for(i=p; i<=q; i++) z(i,r) = rv6(i) * xu;

         x0 = x1;
	} // k=..m1..m2 920 continue

L940: if (q < n) goto L100;
      goto L1001;
//     .......... set error -- non-converged eigenvector ..........
L960: ierr = r; //ierr = 4 * n + r;
      goto L1001;
//     .......... set error -- underestimate of number of
//                eigenvalues in interval ..........
L980: ierr = -1; //ierr = 3 * n + 1;
L1001: lb = t1;
      ub = t2;
      return ierr;
}

int Tsturm(TridiagSym_Array2D<double>& T, TNT::Array1D<double>& W, TNT::Array2D<double>& Z, 
		   double lb, double ub, int& m, TNT::Array2D<double>& g)
/*
Computes some eigenvectors & eigenfunctions of symmetric tridiagonal matrices

  eps: error estimate
  lb,ub	 : lower-upper eigenvalue bounds
  m		 : actual number of eigenvalues found
  W[mm]	 : eigevalues
  Z[mm][n] : contains the eigenvectors. mm numbers the eigenvalue.
  g[7][n] : garbage

  return value:
	0 :		converged
	-1:		if m exceeds mm.
	r:      if the eigenvector corresponding to the r-th
			   eigenvalue fails to converge in 10 iterations.

*/
{
	int n = T.dim(), mm = W.dim();
#ifdef _DEBUG
	assert(n>1);
	assert(Z.dim1()==mm);
	assert(Z.dim2()==n);
	assert(g.dim1()==7);
	assert(g.dim2()==n);
#endif
	int ret = tsturm(n,-1.,
		TNT::Fortran_Array1D<double>(n,T.getd()),
		TNT::Fortran_Array1D<double>(n,T.gets()),
		lb,ub,mm,m,
		TNT::Fortran_Array1D<double>(W),
		TNT::Fortran_Array2D<double>(Z),
		g);
	if (ret>0) m=ret;
	return ret;
}

int Tsturm(TridiagSym_Array2D<float>& T, TNT::Array1D<float>& W, TNT::Array2D<float>& Z, 
		   float lb, float ub, int& m, TNT::Array2D<float>& g)
/*
Computes some eigenvectors & eigenfunctions of symmetric tridiagonal matrices

  eps: error estimate
  lb,ub	 : lower-upper eigenvalue bounds
  m		 : actual number of eigenvalues found
  W[mm]	 : eigevalues
  Z[mm][n] : contains the eigenvectors. mm numbers the eigenvalue.
  g[7][n] : garbage

  return value:
	0 :		converged
	-1:		if m exceeds mm.
	r:      if the eigenvector corresponding to the r-th
			   eigenvalue fails to converge in 10 iterations.

*/
{
	int n = T.dim(), mm = W.dim();
#ifdef _DEBUG
	assert(n>1);
	assert(Z.dim1()==mm);
	assert(Z.dim2()==n);
	assert(g.dim1()==7);
	assert(g.dim2()==n);
#endif
	int ret = tsturm(n,-1.f,
		TNT::Fortran_Array1D<float>(n,T.getd()),
		TNT::Fortran_Array1D<float>(n,T.gets()),
		lb,ub,mm,m,
		TNT::Fortran_Array1D<float>(W),
		TNT::Fortran_Array2D<float>(Z),
		g);
	if (ret>0) m=ret;
	return ret;
}



