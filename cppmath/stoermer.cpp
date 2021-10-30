#include "cppmath.h"

#define MAXSTP 100000
#define KMAXX 8			// Maximum row number used in the extrapolation.
#define IMAXX (KMAXX+1) 
#define SAFE1 0.25		// Safety factors.
#define SAFE2 0.7
#define REDMAX 1.0e-5	// Maximum factor for stepsize reduction.
#define REDMIN 0.7		// Minimum factor for stepsize reduction.
#define TINY 1.0e-30	// Prevents division by zero.
#define SCALMX 0.1		// 1/SCALMX is the maximum factor by which a stepsize can be increased.

template class CBulirschStoer<float>;
template class CBulirschStoer<double>;
template class CStoermer<float>;
template class CStoermer<double>;

template<class T>
void CStoermer<T>::mmid(TNT::Fortran_Array1D<T>& y,	
						  TNT::Fortran_Array1D<T>& d2y, 
						  T xs, T htot, int nstep, 
						  TNT::Fortran_Array1D<T>& yout)
/*
Stoermer’s rule for integrating y'' = f(x, y) for a system of n = nv/2 equations. On input
y[1..nv] contains y in its first n elements and y' in its second n elements, all evaluated at
xs. d2y[1..nv] contains the right-hand side function f (also evaluated at xs) in its first n
elements. Its second n elements are not referenced. Also input is htot, the total step to be
taken, and nstep, the number of substeps to be used. The output is returned as yout[1..nv],
with the same storage arrangement as y. derivs is the user-supplied routine that calculates f.
*/
{
	int i,n,nn;
	float h,h2,halfh,x;
	h=htot/nstep; // Stepsize this trip.
	halfh=0.5*h;
	for (i=1;i<=neqns;i++) { // First step.
		n=neqns+i;
		ytemp(i)=y(i)+(ytemp(n)=h*(y(n)+halfh*d2y(i)));
	}
	x=xs+h;
	odefunc(x,ytemp,yout); // Use yout for temporary storage of derivatives.
	h2=h*h;
	for (nn=2;nn<=nstep;nn++) { // General step.
		for (i=1;i<=neqns;i++)
			ytemp(i) += (ytemp((n=neqns+i)) += h2*yout(i));
		x += h;
		odefunc(x,ytemp,yout);
	}
	for (i=1;i<=neqns;i++) { //Last step.
		n=neqns+i;
		yout(n)=ytemp(n)/h+halfh*yout(i);
		yout(i)=ytemp(i);
	}
}

template<class T>
void CBulirschStoer<T>::mmid(TNT::Fortran_Array1D<T>& y,	
						  TNT::Fortran_Array1D<T>& dydx, 
						  T xs, T htot, int nstep, 
						  TNT::Fortran_Array1D<T>& yout)
/*
Modified midpoint step. At xs, input the dependent variable vector y[1..nvar] and its derivative
vector dydx[1..nvar]. Also input is htot, the total step to be made, and nstep, the
number of substeps to be used. The output is returned as yout[1..nvar], which need not
be a distinct array from y; if it is distinct, however, then y and dydx are returned undamaged.
*/
{
	int n,i;
	float x,swap,h2,h;
	TNT::Fortran_Array1D<T> ym = ytemp, yn = ytemp2;
	h=htot/nstep; // Stepsize this trip.
	for (i=1;i<=nvar;i++) {
		ym(i)=y(i);
		yn(i)=y(i)+h*dydx(i); // First step.
	}
	x=xs+h;
	odefunc(x,yn,yout); // Will use yout for temporary storage of derivatives.
	h2=2.0*h;
	for (n=2;n<=nstep;n++) { // General step.
		for (i=1;i<=nvar;i++) {
			swap=ym(i)+h2*yout(i);
			ym(i)=yn(i);
			yn(i)=swap;
		}
		x += h;
		odefunc(x,yn,yout);
	}
	for (i=1;i<=nvar;i++) // Last step.
		yout(i)=0.5*(ym(i)+yn(i)+h*yout(i));
}

template<class T>
CBulirschStoer<T>::CBulirschStoer(int anvar) : a(IMAXX) , alf(KMAXX,KMAXX), nseq(IMAXX) 
{ 
	Init(anvar);
	for(int i=1; i<=IMAXX; i++) nseq(i) = 2*i;
}

template<class T>
int CBulirschStoer<T>::Init(int anvar)
{
	nvar = anvar;
	if (nvar<1) return (cerror=-1);

	y	  =	TNT::Fortran_Array1D<T>(nvar);
	dydx  = TNT::Fortran_Array1D<T>(nvar);
	yscal = TNT::Fortran_Array1D<T>(nvar);
	yerr  =	TNT::Fortran_Array1D<T>(nvar);
	ytemp = TNT::Fortran_Array1D<T>(nvar);
	ytemp2= TNT::Fortran_Array1D<T>(nvar);
	ysav  = TNT::Fortran_Array1D<T>(nvar);
	yseq  = TNT::Fortran_Array1D<T>(nvar);

	x_	  = TNT::Fortran_Array1D<T>(KMAXX);
	err_  = TNT::Fortran_Array1D<T>(KMAXX);
	d_	  = TNT::Fortran_Array2D<T>(nvar,KMAXX);

	return (cerror=0);
}

template<class T>
void CBulirschStoer<T>::pzextr(int iest, T xest, 
						  TNT::Fortran_Array1D<T>& yest, 
						  TNT::Fortran_Array1D<T>& yz, 
						  TNT::Fortran_Array1D<T>& dy)

/*
Use polynomial extrapolation to evaluate nv functions at x = 0 by .tting a polynomial to a
sequence of estimates with progressively smaller values x = xest, and corresponding function
vectors yest[1..nv]. This call is number iest in the sequence of calls. Extrapolated function
values are output as yz[1..nv], and their estimated error is output as dy[1..nv].
*/
{
	int k1,j;
	float q,f2,f1,delta;
	TNT::Fortran_Array1D<T> c=ytemp;
	x_(iest)=xest; //Save current independent variable.
	for (j=1;j<=nvar;j++) dy(j)=yz(j)=yest(j);
	if (iest == 1) { //Store .rst estimate in .rst column.
		for (j=1;j<=nvar;j++) d_(j,1)=yest(j);
	} else {
		for (j=1;j<=nvar;j++) c(j)=yest(j);
		for (k1=1;k1<iest;k1++) {
			delta=1.0/(x_(iest-k1)-xest);
			f1=xest*delta;
			f2=x_(iest-k1)*delta;
			for (j=1;j<=nvar;j++) { //Propagate tableau 1 diagonal more.
				q=d_(j,k1);
				d_(j,k1)=dy(j);
				delta=c(j)-q;
				dy(j)=f1*delta;
				c(j)=f2*delta;
				yz(j) += dy(j);
			}
		}
		for (j=1;j<=nvar;j++) d_(j,iest)=dy(j);
	}
}

template<class T>
void CBulirschStoer<T>::bsstep(T *xx, T htry, T eps, T *hdid, T *hnext)
/*
Bulirsch-Stoer step with monitoring of local truncation error to ensure accuracy and adjust
stepsize. Input are the dependent variable vector y[1..nv] and its derivative dydx[1..nv]
at the starting value of the independent variable x. Also input are the stepsize to be attempted
htry, the required accuracy eps, and the vector yscal[1..nv] against which the error is
scaled. On output, y and x are replaced by their new values, hdid is the stepsize that was
actually accomplished, and hnext is the estimated next stepsize. derivs is the user-supplied
routine that computes the right-hand side derivatives. Be sure to set htry on successive steps
to the value of hnext returned from the previous step, as is the case if the routine is called
by odeint.
*/
{
	int i,iq,k,kk,km;
	T eps1,errmax,fact,h,red,scale,work,wrkmin,xest;
	int reduct,exitflag=0;

	if (eps != epsold) { // A new tolerance, so reinitialize.
		*hnext = xnew = (T)-1.0e29; // “Impossible” values.
		eps1=SAFE1*eps;
		a(1)=nseq(1)+1; // Compute work coe.cients Ak.
		for (k=1;k<=KMAXX;k++) a(k+1)=a(k)+nseq(k+1);
		for (iq=2;iq<=KMAXX;iq++) { // Compute a(k, q).
			for (k=1;k<iq;k++)
				alf(k,iq)=pow(eps1,(a(k+1)-a(iq+1))/
				((a(iq+1)-a(1)+1.0)*(2*k+1)));
		}
		epsold=eps;
		for (kopt=2;kopt<KMAXX;kopt++) // Determine optimal row number for convergence. 
			if (a(kopt+1) > a(kopt)*alf(kopt-1,kopt)) break;
			kmax=kopt;
	}
	h=htry;
	for (i=1;i<=nvar;i++) ysav(i)=y(i); // Save the starting values.
	if (*xx != xnew || h != (*hnext)) { // A new stepsize or a new integration:
		first=1; // re-establish the order window.
		kopt=kmax;
	}
	reduct=0;
	for (;;) {
		for (k=1;k<=kmax;k++) { // Evaluate the sequence of modi.ed midpoint integrations.
			xnew=(*xx)+h;
			if (xnew == (*xx)) {cerror = 1; return; } // nrerror("step size underflow in bsstep");
			mmid(ysav,dydx,*xx,h,nseq(k),yseq);
			xest=sqrt(h/nseq(k)); // Squared, since error series is even.
			pzextr(k,xest,yseq,y,yerr); // Perform extrapolation.
			if (k != 1) { // Compute normalized error estimate
				errmax=(T)TINY;
				for (i=1;i<=nvar;i++) errmax=max(errmax,(T)fabs(yerr(i)/yscal(i)));
				errmax /= eps; // Scale error relative to tolerance.
				km=k-1;
				err_(km)=pow(errmax/SAFE1,1.0/(2*km+1));
			}
			if (k != 1 && (k >= kopt-1 || first)) { // In order window.
				if (errmax < 1.0) { // Converged.
					exitflag=1;
					break;
				}
				if (k == kmax || k == kopt+1) { // Check for possible stepsize reduction. 
					red=SAFE2/err_(km);
					break;
				}
				else if (k == kopt && alf(kopt-1,kopt) < err_(km)) {
					red=1.0/err_(km);
					break;
				}
				else if (kopt == kmax && alf(km,kmax-1) < err_(km)) {
					red=alf(km,kmax-1)*SAFE2/err_(km);
					break;
				}
				else if (alf(km,kopt) < err_(km)) {
					red=alf(km,kopt-1)/err_(km);
					break;
				}
			}
		}
		if (exitflag) break;
		red=min(red,(T)REDMIN); // Reduce stepsize by at least REDMIN
		red=max(red,(T)REDMAX); // and at most REDMAX.
		h *= red;
		reduct=1;
	} // Try again.
	*xx=xnew; // Successful step taken.
	*hdid=h;
	first=0;
	wrkmin=(T)1.0e35; // Compute optimal row for convergence and corresponding stepsize. 
	for (kk=1;kk<=km;kk++) {
		fact=max(err_(kk),(T)SCALMX);
		work=fact*a(kk+1);
		if (work < wrkmin) {
			scale=fact;
			wrkmin=work;
			kopt=kk+1;
		}
	}
	*hnext=h/scale;
	if (kopt >= k && kopt != kmax && !reduct) {
		// Check for possible order increase, but not if stepsize was just reduced.
		fact=max(scale/alf(kopt-1,kopt),(T)SCALMX);
		if (a(kopt+1)*fact <= wrkmin) {
			*hnext=h/fact;
			kopt++;
		}
	}
}

template<class T>
int CBulirschStoer<T>::Integrate(TNT::Array1D<T>& ystart_c,
						   T x1, T x2, 
						   T eps, T h1, T hmin,
						   int& kp, TNT::Array1D<T>& xp_c, TNT::Array2D<T>& yp_c, T dxsav)
/*
Runge-Kutta driver with adaptive stepsize control. Integrate starting values ystart[1..nvar]
from x1 to x2 with accuracy eps, storing intermediate results in global variables. h1 should
be set as a guessed .rst stepsize, hmin as the minimum allowed stepsize (can be zero). On
output nok and nbad are the number of good and bad (but retried and .xed) steps taken, and
ystart is replaced byv alues at the end of the integration interval. derivs is the user-supplied
routine for calculating the right-hand side derivative, while rkqs is the name of the stepper
routine to be used.
*/
{
	int nstp,i;
	T xsav,x,hnext,hdid,h;

	// reset iterative values & error flag
	cerror = 0;
	first=1;
	kmax=0;
	kopt=0;
	epsold = (T)-1.0;
	xnew = 0;
	
	TNT::Fortran_Array1D<T> ystart(ystart_c);
	TNT::Fortran_Array1D<T> xp(xp_c);
	TNT::Fortran_Array2D<T> yp(yp_c);
	
	x=x1;
	//h=SIGN(h1,x2-x1);
	if (x2>x1) h = fabs(h1); else h = -fabs(h1);
	
	nok = nbad = kount = 0;
	for (i=1;i<=nvar;i++) y(i)=ystart(i);
	if (kp > 0) xsav=x-dxsav*2.0; //Assures storage of .rst step.
	for (nstp=1;nstp<=MAXSTP;nstp++) { //Take at most MAXSTP steps.
		odefunc(x,y,dydx);
		for (i=1;i<=nvar;i++)
			// Scaling used to monitor accuracy. This general-purpose choice can be modi.ed if need be.
			yscal(i)=fabs(y(i))+fabs(dydx(i)*h)+(T)TINY;
		if (kp > 0 && kount < kp-1 && fabs(x-xsav) > fabs(dxsav)) {
			xp(++kount)=x; //Store intermediate results.
			for (i=1;i<=nvar;i++) yp(i,kount)=y(i);
			xsav=x;
		}
		if ((x+h-x2)*(x+h-x1) > 0.0) h=x2-x; //If stepsize can overshoot, decrease.
		bsstep(&x, h, eps, &hdid, &hnext);
		if (cerror!=0) return cerror;
		if (hdid == h) ++nok; else ++nbad;
		if ((x-x2)*(x2-x1) >= 0.0) { // Are we done?
			for (i=1;i<=nvar;i++) ystart(i)=y(i);
			if (kp) {
				xp(++kount)=x; // Save .nal step.
				for (i=1;i<=nvar;i++) yp(i,kount)=y(i);
			}
			return cerror; // Normal exit.
		}
		if (fabs(hnext) <= hmin) { cerror = 2; return cerror; } // nrerror("Step size too small in odeint");
		h=hnext;
	}
	cerror = 3; return cerror;
	//nrerror("Too many steps in routine odeint");
}

template<class T>
int CBulirschStoer<T>::Integrate(TNT::Array1D<T>& ystart_c,
						   T x1, T x2, 
						   T eps, T h1, T hmin)
/*
Runge-Kutta driver with adaptive stepsize control. Integrate starting values ystart[1..nvar]
from x1 to x2 with accuracy eps, storing intermediate results in global variables. h1 should
be set as a guessed .rst stepsize, hmin as the minimum allowed stepsize (can be zero). On
output nok and nbad are the number of good and bad (but retried and .xed) steps taken, and
ystart is replaced byv alues at the end of the integration interval. derivs is the user-supplied
routine for calculating the right-hand side derivative, while rkqs is the name of the stepper
routine to be used.
*/
{
	int nstp,i;
	T x,hnext,hdid,h;
	
	// reset iterative values & error flag
	cerror = 0;
	first=1;
	kmax=0;
	kopt=0;
	epsold = (T)-1.0;
	xnew = 0;

	TNT::Fortran_Array1D<T> ystart(ystart_c);
	
	x=x1;
	//h=SIGN(h1,x2-x1);
	if (x2>x1) h = fabs(h1); else h = -fabs(h1);
	
	nok = nbad = kount = 0;
	for (i=1;i<=nvar;i++) y(i)=ystart(i);
	for (nstp=1;nstp<=MAXSTP;nstp++) { //Take at most MAXSTP steps.
		odefunc(x,y,dydx);
		for (i=1;i<=nvar;i++)
			// Scaling used to monitor accuracy. This general-purpose choice can be modi.ed if need be.
			yscal(i)=fabs(y(i))+fabs(dydx(i)*h)+(T)TINY;
		if ((x+h-x2)*(x+h-x1) > 0.0) h=x2-x; //If stepsize can overshoot, decrease.
		bsstep(&x, h, eps, &hdid, &hnext);
		if (cerror!=0) return cerror;
		if (hdid == h) ++nok; else ++nbad;
		if ((x-x2)*(x2-x1) >= 0.0) { // Are we done?
			for (i=1;i<=nvar;i++) ystart(i)=y(i);
			return cerror; // Normal exit.
		}
		if (fabs(hnext) <= hmin) { cerror = 2; return cerror; } // nrerror("Step size too small in odeint");
		h=hnext;
	}
	cerror = 3; return cerror;
	//nrerror("Too many steps in routine odeint");
}

