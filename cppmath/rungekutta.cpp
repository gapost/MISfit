#include "cppmath.h"

#define SAFETY 0.9
#define PGROW -0.2
#define PSHRNK -0.25
#define ERRCON 1.89e-4
//The value ERRCON equals (5/SAFETY) raised to the power (1/PGROW), see use below.

#define MAXSTP 100000
#define TINY 1.0e-30

template class CRungeKutta<float>;
template class CRungeKutta<double>;

template<class T>
int CRungeKutta<T>::Init(int anvar)
{
	nvar = anvar;
	if (nvar<1) return (cerror=-1);

	//v	  = TNT::Array1D<float>(nvar);
	vout= TNT::Array1D<T>(nvar);
	dv  = TNT::Array1D<T>(nvar);

	dym = TNT::Array1D<T>(nvar);
	dyt = TNT::Array1D<T>(nvar);
	yt =  TNT::Array1D<T>(nvar);

	return (cerror=0);
}

template<class T>
void CRungeKutta<T>::rk4(TNT::Array1D<T>& y, TNT::Array1D<T>& dydx, 
					  T x, T h, TNT::Array1D<T>& yout)
/*
Given values for the variables y[1..n] and their derivatives dydx[1..n] known at x, use the
fourth-order Runge-Kutta method to advance the solution over an interval h and return the
incremented variables as yout[1..n], which need not be a distinct array from y. The user
supplies the routine derivs(x,y,dydx), which returns derivatives dydx at x.
*/
{
	int i;
	T xh,hh,h6;
	hh=h*0.5;
	h6=h/6.0;
	xh=x+hh;
	for (i=0;i<nvar;i++) yt[i]=y[i]+hh*dydx[i]; //First step.
	odefunc(xh,yt,dyt); //Second step.
	for (i=0;i<nvar;i++) yt[i]=y[i]+hh*dyt[i];
	odefunc(xh,yt,dym); //Third step.
	for (i=0;i<nvar;i++) {
		yt[i]=y[i]+h*dym[i];
		dym[i] += dyt[i];
	}
	odefunc(x+h,yt,dyt); //Fourth step.
	for (i=0;i<nvar;i++) //Accumulate increments with proper weights. 
		yout[i]=y[i]+h6*(dydx[i]+dyt[i]+2.0*dym[i]);
}

template<class T>
int CRungeKutta<T>::Integrate(TNT::Array1D<T>& v, T x1, T x2, int nstep)
/*
Starting from initial values vstart[1..nvar] known at x1 use fourth-order Runge-Kutta
to advance nstep equal increments to x2. The user-supplied routine derivs(x,v,dvdx)
evaluates derivatives. Results are stored in the global variables y[1..nvar][1..nstep+1]
and xx[1..nstep+1].
*/
{
	int i,k;
	T x,h;
	/*for (i=0;i<nvar;i++) { //Load starting values.
		v[i]=vstart[i];
		//y[i][0]=v[i];
	}*/
	//xx[0]=x1;
	x=x1;
	h=(x2-x1)/nstep;
	for (k=1;k<=nstep;k++) { //Take nstep steps.
		odefunc(x,v,dv);
		rk4(v,dv,x,h,vout);
		if ((T)(x+h) == x) return (cerror=1); //nrerror("Step size too small in routine rkdumb");
		x += h;
		//xx[k]=x; //Store intermediate steps.
		for (i=0;i<nvar;i++) {
			v[i]=vout[i];
			//y[i][k]=v[i];
		}
	}
	return 0;
}

template<class T>
int CRungeKutta<T>::Integrate(TNT::Array1D<T>& v, T x1, T x2, int nstep, TNT::Array1D<T>& xx, TNT::Array2D<T>& y)
/*
Starting from initial values vstart[1..nvar] known at x1 use fourth-order Runge-Kutta
to advance nstep equal increments to x2. The user-supplied routine derivs(x,v,dvdx)
evaluates derivatives. Results are stored in the global variables y[1..nvar][1..nstep+1]
and xx[1..nstep+1].
*/
{
	int i,k;
	T x,h;
	for (i=0;i<nvar;i++) { //Load starting values.
		y[i][0]=v[i];
	}
	xx[0]=x1;
	x=x1;
	h=(x2-x1)/(nstep-1);
	for (k=1;k<nstep;k++) { //Take nstep steps.
		odefunc(x,v,dv);
		rk4(v,dv,x,h,vout);
		if ((T)(x+h) == x) return (cerror=1); //nrerror("Step size too small in routine rkdumb");
		x += h;
		xx[k]=x; //Store intermediate steps.
		for (i=0;i<nvar;i++) {
			v[i]=vout[i];
			y[i][k]=v[i];
		}
	}
	return 0;
}


template class CAdptRungeKutta<float>;
template class CAdptRungeKutta<double>;


template<class T>
int CAdptRungeKutta<T>::Init(int anvar)
{
	nvar = anvar;
	if (nvar<1) return (cerror=-1);

	yscal = TNT::Array1D<T>(nvar);
	y	  =	TNT::Array1D<T>(nvar);
	dydx  = TNT::Array1D<T>(nvar);

	yerr =	TNT::Array1D<T>(nvar);
	ytemp = TNT::Array1D<T>(nvar);
	ak2 =	TNT::Array1D<T>(nvar);
	ak3 =	TNT::Array1D<T>(nvar);
	ak4 =	TNT::Array1D<T>(nvar);
	ak5 =	TNT::Array1D<T>(nvar);
	ak6 =	TNT::Array1D<T>(nvar);

	return (cerror=0);
}

/*
Runge-Kutta driver with adaptive stepsize control. Integrate starting values ystart[1..nvar]
from x1 to x2 with accuracy eps, storing intermediate results in global variables. h1 should
be set as a guessed .rst stepsize, hmin as the minimum allowed stepsize (can be zero). On
output nok and nbad are the number of good and bad (but retried and .xed) steps taken, and
ystart is replaced byv alues at the end of the integration interval. derivs is the user-supplied
routine for calculating the right-hand side derivative, while rkqs is the name of the stepper
routine to be used.
*/
/*
extern int kmax,kount;
extern float *xp,**yp,dxsav;
User storage for intermediate results. Preset kmax and dxsav in the calling program. If kmax .=
0 results are stored at approximate intervals dxsav in the arrays xp[1..kount], yp[1..nvar]
[1..kount], where kount is output by odeint. De.ning declarations for these variables, with
memory allocations xp[1..kmax] and yp[1..nvar][1..kmax] for the arrays, should be in
the calling program.
*/
template<class T>
int CAdptRungeKutta<T>::Integrate(TNT::Array1D<T>& ystart,
						   T x1, T x2, 
						   T eps, T h1, T hmin,
						   int& kmax, TNT::Array1D<T>& xp, TNT::Array2D<T>& yp, T dxsav)
{
	//if (cerror!=0) return cerror;
	assert(cerror==0);

	int nstp,i;
	T xsav,x,hnext,hdid,h;

	x=x1;

	//h=SIGN(h1,x2-x1);
	if (x2>x1) h = fabs(h1); else h = -fabs(h1);
	
	nok = nbad = kount = 0;
	for (i=0;i<nvar;i++) y[i]=ystart[i];
	if (kmax > 0) xsav=x-dxsav*2.0; //Assures storage of first step.
	for (nstp=1;nstp<=MAXSTP;nstp++) { //Take at most MAXSTP steps.
		odefunc(x,y,dydx);
		for (i=0;i<nvar;i++)
			//Scaling used to monitor accuracy. This general-purpose choice can be modified if need be.
			yscal[i]=fabs(y[i])+fabs(dydx[i]*h)+TINY;
		if (kmax > 0 && kount < kmax && fabs(x-xsav) > fabs(dxsav)) {
			xp[kount]=x; //Store intermediate results.
			for (i=0;i<nvar;i++) yp[i][kount]=y[i];
			kount++;
			xsav=x;
		}
		if ((x+h-x2)*(x+h-x1) > 0.0) h=x2-x; //If stepsize can overshoot, decrease.
		if (rkqs(&x,h,eps,&hdid,&hnext)!=0) return cerror;
		if (hdid == h) nok++; else nbad++;
		if ((x-x2)*(x2-x1) >= 0.0) { //Are we done?
			for (i=0;i<nvar;i++) ystart[i]=y[i];
			if (kmax) {
				xp[kount]=x; //Save .nal step.
				for (i=0;i<nvar;i++) yp[i][kount]=y[i];
				kount++;
			}
			kmax = kount;
			return (cerror=0); //Normal exit.
		}
		if (fabs(hnext) <= hmin) return (cerror=1); //nrerror("Step size too small in odeint");
		h=hnext;
	}
	return (cerror=2); //nrerror("Too many steps in routine odeint");
}

template<class T>
int CAdptRungeKutta<T>::Integrate(TNT::Array1D<T>& ystart,
						   T x1, T x2, 
						   T eps, T h1, T hmin)
{
	if (cerror!=0) return cerror;

	int nstp,i;
	T x,hnext,hdid,h;

	x=x1;

	//h=SIGN(h1,x2-x1);
	if (x2>x1) h = fabs(h1); else h = -fabs(h1);
	
	nok = nbad = kount = 0;
	for (i=0;i<nvar;i++) y[i]=ystart[i];
	for (nstp=1;nstp<=MAXSTP;nstp++) { //Take at most MAXSTP steps.
		odefunc(x,y,dydx);
		for (i=0;i<nvar;i++)
			//Scaling used to monitor accuracy. This general-purpose choice can be modified if need be.
			yscal[i]=fabs(y[i])+fabs(dydx[i]*h)+TINY;
		if ((x+h-x2)*(x+h-x1) > 0.0) h=x2-x; //If stepsize can overshoot, decrease.
		if (rkqs(&x,h,eps,&hdid,&hnext)!=0) return cerror;
		if (hdid == h) nok++; else nbad++;
		if ((x-x2)*(x2-x1) >= 0.0) { //Are we done?
			for (i=0;i<nvar;i++) ystart[i]=y[i];
			return (cerror=0); //Normal exit.
		}
		if (fabs(hnext) <= hmin) return (cerror=1); //nrerror("Step size too small in odeint");
		h=hnext;
	}
	return (cerror=2); //nrerror("Too many steps in routine odeint");
}

template<class T>
int CAdptRungeKutta<T>::rkqs(T *x, T htry, T eps,
		  T *hdid, T *hnext)
/*
Fifth-order Runge-Kutta step with monitoring of local truncation error to ensure accuracya nd
adjust stepsize. Input are the dependent variable vector y[1..n] and its derivative dydx[1..n]
at the starting value of the independent variable x. Also input are the stepsize to be attempted
htry, the required accuracy eps, and the vector yscal[1..n] against which the error is
scaled. On output, y and x are replaced bythei r new values, hdid is the stepsize that was
actuallyac complished, and hnext is the estimated next stepsize. derivs is the user-supplied
routine that computes the right-hand side derivatives.
*/
{
	int i;
	T errmax,h,htemp,xnew;
	h=htry; //Set stepsize to the initial trial value.
	for (;;) {
		rkck(*x,h); //Take a step. ytemp, yerr updated 
		errmax=0.0; //Evaluate accuracy.
		for (i=0;i<nvar;i++) errmax=max(errmax,(T)fabs(yerr[i]/yscal[i]));
		errmax /= eps; //Scale relative to required tolerance.
		if (errmax <= 1.0) break; //Step succeeded. Compute size of next step.
		htemp=SAFETY*h*pow(errmax,PSHRNK);
		//Truncation error too large, reduce stepsize.
		h=(h >= 0.0 ? max(htemp,0.1f*h) : min(htemp,0.1f*h));
		//No more than a factor of 10.
		xnew=(*x)+h;
		if (xnew == *x) return (cerror=3); //nrerror("stepsize underflow in rkqs");
	}
	if (errmax > ERRCON) *hnext=SAFETY*h*pow(errmax,PGROW);
	else *hnext=5.0*h; //No more than a factor of 5 increase.
	*x += (*hdid=h);
	for (i=0;i<nvar;i++) y[i]=ytemp[i];
	return 0;
}

template<class T>
void CAdptRungeKutta<T>::rkck(T x, T h)
/*
Given values for n variables y[1..n] and their derivatives dydx[1..n] known at x, use
the .fth-order Cash-Karp Runge-Kutta method to advance the solution over an interval h
and return the incremented variables as yout[1..n]. Also return an estimate of the local
truncation error in yout using the embedded fourth-order method. The user supplies the routine
derivs(x,y,dydx), which returns derivatives dydx at x.
*/
{
	int i;
	const float a2=0.2f,a3=0.3f,a4=0.6f,a5=1.0f,a6=0.875f,b21=0.2f,
		b31=3.0f/40.0f,b32=9.0f/40.0f,b41=0.3f,b42 = -0.9f,b43=1.2f,
		b51 = -11.0f/54.0f, b52=2.5f,b53 = -70.0f/27.0f,b54=35.0f/27.0f,
		b61=1631.0f/55296.0f,b62=175.0f/512.0f,b63=575.0f/13824.0f,
		b64=44275.0f/110592.0f,b65=253.0f/4096.0f,c1=37.0f/378.0f,
		c3=250.0f/621.0f,c4=125.0f/594.0f,c6=512.0f/1771.0f,
		dc5 = -277.00f/14336.0f;
	const float dc1=c1-2825.0f/27648.0f,dc3=c3-18575.0f/48384.0f,
		dc4=c4-13525.0f/55296.0f,dc6=c6-0.25f;

	for (i=0;i<nvar;i++) //First step.
		ytemp[i]=y[i]+b21*h*dydx[i];
	odefunc(x+a2*h,ytemp,ak2); //Second step.
	for (i=0;i<nvar;i++)
		ytemp[i]=y[i]+h*(b31*dydx[i]+b32*ak2[i]);
	odefunc(x+a3*h,ytemp,ak3); //Third step.
	for (i=0;i<nvar;i++)
		ytemp[i]=y[i]+h*(b41*dydx[i]+b42*ak2[i]+b43*ak3[i]);
	odefunc(x+a4*h,ytemp,ak4); //Fourth step.
	for (i=0;i<nvar;i++)
		ytemp[i]=y[i]+h*(b51*dydx[i]+b52*ak2[i]+b53*ak3[i]+b54*ak4[i]);
	odefunc(x+a5*h,ytemp,ak5); //Fifth step.
	for (i=0;i<nvar;i++)
		ytemp[i]=y[i]+h*(b61*dydx[i]+b62*ak2[i]+b63*ak3[i]+b64*ak4[i]+b65*ak5[i]);
	odefunc(x+a6*h,ytemp,ak6); //Sixth step.
	for (i=0;i<nvar;i++) //Accumulate increments with proper weights.
		ytemp[i]=y[i]+h*(c1*dydx[i]+c3*ak3[i]+c4*ak4[i]+c6*ak6[i]);
	for (i=0;i<nvar;i++)
		yerr[i]=h*(dc1*dydx[i]+dc3*ak3[i]+dc4*ak4[i]+dc5*ak5[i]+dc6*ak6[i]);
	//Estimate error as difference between fourth and fifth order methods.
}

