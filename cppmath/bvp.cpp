#include "cppmath.h"
#include <math.h>

CBVRelax::CBVRelax(int ane, int am, int anb) : ne(ane), m(am), nb(anb)
{
	indexv = TNT::Fortran_Array1D<int>(ne);
	for(int i = 1; i<=ne; i++) indexv(i) = i;
	scalv = TNT::Fortran_Array1D<double>(ne,1.);

	c = TNT::Fortran_Array3D<double>(ne,ne-nb+1,m+1);
	s = TNT::Fortran_Array2D<double>(ne,2*ne+1);
	
	kmax = TNT::Fortran_Array1D<int>(ne);
	ermax = TNT::Fortran_Array1D<int>(ne);
}

int CBVRelax::Solve(int itmax, double conv, double slowc, TNT::Fortran_Array2D<double>& y)
/*
Driver routine for solution of two point boundary value problems by relaxation. itmax is the
maximum number of iterations. conv is the convergence criterion (see text). slowc controls
the fraction of corrections actually used after each iteration. scalv[1..ne] contains typical
sizes for each dependent variable, used to weight errors. indexv[1..ne] lists the column
ordering of variables used to construct the matrix s[1..ne][1..2*ne+1] of derivatives. (The
nb boundary conditions at the first mesh point must contain some dependence on the first nb
variables listed in indexv.) The problem involves ne equations for ne adjustable dependent
variables at each point. At the fisrt mesh point there are nb boundary conditions. There are a
total of m mesh points. y[1..ne][1..m] is the two-dimensional array that contains the initial
guess for all the dependent variables at each mesh point. On each iteration, it is updated by the
calculated correction. The arrays c[1..ne][1..ne-nb+1][1..m+1] and s supply dummy
storage used by the relaxation code.
*/
{
	int ic1,ic2,ic3,ic4,it,j,j1,j2,j3,j4,j5,j6,j7,j8,j9;
	int jc1,jcf,jv,k,k1,k2,km,kp,nvars,ret;
	double errj,fac,vmax,vz;


	k1=1; //Set up row and column markers.
	k2=m;
	nvars=ne*m;
	j1=1;
	j2=nb;
	j3=nb+1;
	j4=ne;
	j5=j4+j1;
	j6=j4+j2;
	j7=j4+j3;
	j8=j4+j4;
	j9=j8+j1;
	ic1=1;
	ic2=ne-nb;
	ic3=ic2+1;
	ic4=ne;
	jc1=1;
	jcf=ic3;
	for (it=1;it<=itmax;it++) { //Primary iteration loop.
		k=k1; //Boundary conditions at first point.
		Difeq(k,k1,k2,j9,ic3,ic4,y);
		if ((ret=pinvs(ic3,ic4,j5,j9,jc1,k1))!=CPPMATH_OK) return ret;
		for (k=k1+1;k<=k2;k++) { //Finite diference equations at all point pairs.
			kp=k-1;
			Difeq(k,k1,k2,j9,ic1,ic4,y);
			red(ic1,ic4,j1,j2,j3,j4,j9,ic3,jc1,jcf,kp);
			if ((ret=pinvs(ic1,ic4,j3,j9,jc1,k))!=CPPMATH_OK) return ret;
		}
		k=k2+1; //Final boundary conditions.
		Difeq(k,k1,k2,j9,ic1,ic2,y);
		red(ic1,ic2,j5,j6,j7,j8,j9,ic3,jc1,jcf,k2);
		if ((ret=pinvs(ic1,ic2,j7,j9,jcf,k2+1))!=CPPMATH_OK) return ret;
		bksub(ne,nb,jcf,k1,k2); //Backsubstitution.
		err=0.0;
		for (j=1;j<=ne;j++) { //Convergence check, accumulate average error.
			jv=indexv(j);
			errj=vmax=0.0;
			km=0;
			for (k=k1;k<=k2;k++) { //Find point with largest error, for each dependent variable. 
				vz=fabs(c(jv,1,k));
				if (vz > vmax) {
					vmax=vz;
					km=k;
				}
				errj += vz;
			}
			err += errj/scalv(j); //Note weighting for each dependent variable.
			ermax(j)=c(jv,1,km)/scalv(j);
			
			kmax(j)=km;
		}
		err /= nvars;
		fac=(err > slowc ? slowc/err : 1.0);
		//Reduce correction applied when error is large.
		for (j=1;j<=ne;j++) { //Apply corrections.
			jv=indexv(j);
			for (k=k1;k<=k2;k++)
				y(j,k) -= fac*c(jv,1,k);
		}
#ifdef _DEBUG
		//printf("\n%8s %9s %9s\n","Iter.","Error","FAC"); //Summary of corrections for this step. 
		//printf("%6d %12.6f %11.6f\n",it,err,fac);
		cout << "Iter " << it << "\tErr " << err << "\tFAC " << fac << endl;
#endif
		if (err < conv) { //Point with largest error for each variable can
			//be monitored by writing out kmax and ermax.
			return CPPMATH_OK;
		}
	}
	return CPPMATH_ERR_ITER; //nrerror("Too many iterations in solvde"); Convergence failed.
}

void CBVRelax::bksub(int ne, int nb, int jf, int k1, int k2)
//Backsubstitution, used internally by solvde.
{
	int nbf,im,kp,k,j,i;
	double xx;
	nbf=ne-nb;
	im=1;
	for (k=k2;k>=k1;k--) { //Use recurrence relations to eliminate remaining dependences.
		if (k == k1) im=nbf+1;
		kp=k+1;
		for (j=1;j<=nbf;j++) {
			xx=c(j,jf,kp);
			for (i=im;i<=ne;i++)
				c(i,jf,k) -= c(i,j,k)*xx;
		}
	}
	for (k=k1;k<=k2;k++) { //Reorder corrections to be in column 1.
		kp=k+1;
	for (i=1;i<=nb;i++) c(i,1,k)=c(i+nbf,jf,k);
	for (i=1;i<=nbf;i++) c(i+nb,1,k)=c(i,jf,kp);
	}
}

int CBVRelax::pinvs(int ie1, int ie2, int je1, int jsf, int jc1, int k)
/*
Diagonalize the square subsection of the s matrix, and store the recursion coefficients in c;
used internally by solvde.
*/
{
	int js1,jpiv,jp,je2,jcoff,j,irow,ipiv,id,icoff,i;
	double pivinv,piv,dum,big;
	
	TNT::Fortran_Array1D<int> indxr(ie2-ie1+1);
	TNT::Fortran_Array1D<int> pscl(ie2-ie1+1);
	
	je2=je1+ie2-ie1;
	js1=je2+1;
	
	for (i=ie1;i<=ie2;i++) { //Implicit pivoting, as in §2.1.
		big=0.0;
		for (j=je1;j<=je2;j++)
			if (fabs(s(i,j)) > big) big=fabs(s(i,j));
		if (big == 0.0) return -1; //nrerror("Singular matrix - row all 0, in pinvs");
		pscl(i-ie1+1)=1.0/big;
		indxr(i-ie1+1)=0;
	}
	for (id=ie1;id<=ie2;id++) {
		piv=0.0;
		for (i=ie1;i<=ie2;i++) { //Find pivot element.
			if (indxr(i-ie1+1) == 0) {
				big=0.0;
				for (j=je1;j<=je2;j++) {
					if (fabs(s(i,j)) > big) {
						jp=j;
						big=fabs(s(i,j));
					}
				}
				if (big*pscl(i-ie1+1) > piv) {
					ipiv=i;
					jpiv=jp;
					piv=big*pscl(i-ie1+1);
				}
			}
		}
		if (s(ipiv,jpiv) == 0.0) return -1; // nrerror("Singular matrix in routine pinvs");
		indxr(ipiv-ie1+1)=jpiv; //In place reduction. Save column ordering.
		pivinv=1.0/s(ipiv,jpiv);
		for (j=je1;j<=jsf;j++) s(ipiv,j) *= pivinv; //Normalize pivot row.
		s(ipiv,jpiv)=1.0;
		for (i=ie1;i<=ie2;i++) { //Reduce nonpivot elements in column.
			if (indxr(i-ie1+1) != jpiv) {
				if (s(i,jpiv)) {
					dum=s(i,jpiv);
					for (j=je1;j<=jsf;j++)
						s(i,j) -= dum*s(ipiv,j);
					s(i,jpiv)=0.0;
				}
			}
		}
	}
	jcoff=jc1-js1; //Sort and store unreduced coe.cients.
	icoff=ie1-je1;
	for (i=ie1;i<=ie2;i++) {
		irow=indxr(i-ie1+1)+icoff;
		for (j=js1;j<=jsf;j++) c(irow,j+jcoff,k)=s(i,j);
	}
	return 0;
}

void CBVRelax::red(int iz1, int iz2, int jz1, int jz2, int jm1, int jm2, int jmf,
		 int ic1, int jc1, int jcf, int kc)
/*
Reduce columns jz1-jz2 of the s matrix, using previous results as stored in the c matrix. Only
columns jm1-jm2,jmf are a.ected by the prior results. red is used internally by solvde.
*/
{
	int loff,l,j,ic,i;
	float vx;
	loff=jc1-jm1;
	ic=ic1;
	
	for (j=jz1;j<=jz2;j++) { //Loop over columns to be zeroed.
		for (l=jm1;l<=jm2;l++) { //Loop over columns altered.
			vx=c(ic,l+loff,kc);
			for (i=iz1;i<=iz2;i++) s(i,l) -= s(i,j)*vx; //Loop over rows.
		}
		vx=c(ic,jcf,kc);
		for (i=iz1;i<=iz2;i++) s(i,jmf) -= s(i,j)*vx; //Plus final element.
		ic += 1;
	}
}

/******************** CBVDE2 ************************************/
/* 2nd order diff. equation solver */
/*
int CBVDE2::Solve(float x0, float x1, float y0, float y1,
				  int itmax, float conv, float slowc, int m, TNT::Array2D<float>& y_c)
{
	TNT::Array1D<float> scalv(2,1.f);
	TNT::Array1D<int> indexv(2);
	indexv[0] = 1; indexv[1] = 2;
	x1_ = x1; x0_ = x0;
	y1_ = y1; y0_ = y0;
	m_ = m;
	h = (x1-x0)/(m-1);
	return CBVRelax::Solve(itmax,conv,slowc,scalv,indexv,2,1,m,y_c);
}
	
void CBVDE2::Difeq(int k, int k1, int k2, int jsf, int is1, int isf,
					 TNT::Fortran_Array1D<int>& indexv, int ne, 
					 TNT::Fortran_Array2D<float>& s, TNT::Fortran_Array2D<float>& y)
{
	if (k==k1) {
		s(2,3) = 1.f;
		s(2,4) = 0.f;
		s(2,jsf) = y(1,1) - y0_;
	} else if (k>k2) {
		s(1,3) = 1.f;
		s(1,4) = 0.f;
		s(1,jsf) = y(1,m_) - y1_;
	} else {
		TNT::Array1D<float> g1(3), g2(3);
		G(x0_+(k-1)*h,y(1,k),y(2,k),g1);
		G(x0_+(k-2)*h,y(1,k-1),y(2,k-1),g2);

		s(1,1) = -1.f; 
		s(1,2) = -h/2;
		s(1,3) = 1; 
		s(1,4) = -h/2;
		s(1,jsf) = (y(1,k) - y(1,k-1)) - h/2*(y(2,k) + y(2,k-1));

		s(2,1) = -h/2*g1[1]; 
		s(2,2) = -1.f - h*g1[2]/2;
		s(2,3) = -h*g2[1]; 
		s(2,4) = 1.f - h*g2[2]/2;
		s(2,jsf) = (y(2,k)-y(2,k-1)) - h*(g1[0]+g2[0])/2;
	}
}
*/