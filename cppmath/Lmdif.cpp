/* lmdif.c -- levenberg-marquardt algorithm */
#include <math.h>
#include "cppmath.h"
#include "funcs.h"

CLMDif::CLMDif(int an, int am, LMDif_MinFunc aFunc)
{
   if ( an <= 0 || am < an || aFunc==0) {
       info = -1;
       return;
   }

   m = am;
   n = an;
   MinFunc = aFunc;

   initialize_();
}

void CLMDif::initialize_()
{
	params = CArray1D(n,0.);
	params_store = CArray1D(n,0.);
	msk = CIntArray1D(n,0);
	diag = CArray1D(n,1.);
	fjac = CArray2D(n,m,0.); 
	fvec = CArray1D(m,0.);
	qtf = CArray1D(n,0.);
	wa1 = CArray1D(n,0.);
	wa2 = CArray1D(n,0.);
	wa3 = CArray1D(n,0.);
	wa4 = CArray1D(m,0.);
	ipvt = CIntArray1D(n,0);

	cookie = 0;

	info = 0;
}

void CLMDif::Init(const CArray1D& ax, const CIntArray1D& amsk, double tol)
{
/* Check input parameters */
   if (tol < 0.0 || ax.dim()!=n || amsk.dim()!=n) {
       info = -1;
       return;
   }

/* Set convergence tolerances */
    ftol = tol;
    xtol = tol;
    gtol = 0.0;

    maxfev = 800;
    epsfcn = 0.0;
    mode = 1;
    factor = 10.;
    nfev = 0;

    Init(ax,amsk,ftol,xtol,gtol,maxfev,epsfcn,factor);
}

void CLMDif::Init(const CArray1D& aparams, const CIntArray1D& amsk, 
				  double aftol, double axtol,double agtol,int amaxfev, 
				  double aepsfcn, double afactor)
{
/* check for input parameter errors */
    if ((amaxfev <= 0) || (afactor <= 0)) {
		info = -1;
		return;
	}

	params.inject(aparams);
	params_store.inject(aparams);
	msk.inject(amsk);

	// define scaling mode
	SetScales(diag);
	mode = 1;
	int i = 0;
	while( (mode==1) && (i<n) ) if (diag[i++]!=1.) mode = 2;

	ftol = aftol;
	xtol = axtol;
	gtol = agtol;
	maxfev = amaxfev;
	epsfcn = aepsfcn;
	factor = afactor;

	LoopInit_();
}

double CLMDif::GetError()
{
	MinFunc(params.getdata(),fvec.getdata(),cookie);
	return enorm(m,fvec);
}

void CLMDif::LoopInit_()
{
/* initialize */
    epsmch = DPMPAR[0];

    info = 0;
    iflag = 0;
    nfev = 0;

    if (mode == 2) {
        for (int j = 0; j < n; j++)
            if (diag[j] <= 0) {
				info =-1;
				return;
			}
    }

/* evaluate the function at the starting point and calculate its norm */   
    iflag = MinFunc(params.getdata(),fvec.getdata(),cookie);
    nfev = 1;
    if (iflag < 0) {
        info = iflag;
        return;
    }
    fnorm = enorm(m,fvec);

/* initialize levenberg-marquardt counters */
    par = 0;
    iter = 1;
	LoopID = 0;
}

void CLMDif::Loop0_()
{
	iflag = 2;
	fdjac2(params,fvec,fjac,iflag,epsfcn,wa4);
	nfev += n;
	if (iflag < 0) {
		info = iflag;
		return;
	}
    iflag = MinFunc(params.getdata(),fvec.getdata(),cookie);
	if (iflag < 0) {
		info = iflag;
		return;
	} 
	qrfac(m,n,fjac,1,ipvt,wa1,wa2,wa3);                
	if (iter == 1) {
		if (mode != 2) {
			for (j = 0;j < n; j++) {
				diag[j] = wa2[j];
				if (wa2[j] == 0.0) diag[j] = 1.0;
			}
		}
		for (j = 0;j < n; j++)
			wa3[j] = diag[j] * params[j];
		xnorm = enorm(n,wa3);
		delta = factor * xnorm;
		if (delta == 0) delta = factor;
	}
	for (i = 0; i < m; i++)
		wa4[i] = fvec[i];
	for (j = 0;j < n; j++) {
		if (fjac[j][j] != 0.0) {
			sum = 0.0;
			for (i = j;i < m; i++)
				sum += fjac[j][i] * wa4[i];
			temp = -sum / fjac[j][j];
			for (i = j; i < m; i++)
				wa4[i] += fjac[j][i] * temp;
		}
		fjac[j][j] = wa1[j];
		qtf[j] = wa4[j];
	}
	/* compute the norm of the scaled gradient */ 
	gnorm = 0.0;
	if (fnorm != 0.0) {
		for (j = 0; j < n; j++) {
			l = ipvt[j];
			if (wa2[l] == 0.0) continue;
			sum = 0.0;
			for (i = l; i <= j; i++)
				sum += fjac[j][i] * qtf[i] / fnorm;
			gnorm = max(gnorm,fabs(sum/wa2[l]));
		}
	}
	/* test for convergence of the gradient norm */
	if (gnorm <= gtol) info = 4;
	if (info != 0) {
		info = iflag;
		return;
	}
	/* rescale if necessary */
	if (mode != 2) {
        for (j = 0; j < n; j++)
            diag[j] = max(diag[j],wa2[j]);
	}
	LoopID = 1;
}

void CLMDif::Loop1_()
{
	lmpar(n,fjac,ipvt,diag,qtf,delta,&par,wa1,wa2,wa3,wa4);
	for (j = 0;j < n; j++) {
		wa1[j] = -wa1[j];
		wa2[j] = params[j] + wa1[j];
		wa3[j] = diag[j] * wa1[j];
	}
	pnorm = enorm(n,wa3);
	if (iter == 1) delta = min(delta,pnorm);
    iflag = MinFunc(wa2.getdata(),wa4.getdata(),cookie);
	nfev++;
	if (iflag < 0) {
		info = iflag;
		return;
	}
	fnorm1 = enorm(m,wa4);
	actred = -1.0;
	if (0.1 * fnorm1 < fnorm)
		actred = 1.0 - (fnorm1*fnorm1/(fnorm*fnorm));
	for (j = 0;j < n; j++) {
		wa3[j] = 0.0;
		l = ipvt[j];
		temp = wa1[l];
		for (i = 0; i <= j; i++)
			wa3[i] += fjac[j][i] * temp;
	}
	temp1 = enorm(n,wa3) / fnorm;
	temp2 = sqrt(par) * pnorm / fnorm;
	prered = temp1*temp1 + temp2*temp2 / 0.5;
	dirder = -(temp1*temp1 + temp2*temp2);
	ratio = 0.0;
	if (prered != 0.0) ratio = actred/prered;
	if (ratio <= 0.25) {
		if (actred > 0.0) temp = 0.5;
		if (actred < 0.0) temp = 0.5*dirder/(dirder+0.5*actred);
		delta = temp * min(delta,pnorm/0.1);
		par /= temp;
	}
	else {
		if ((par == 0.0) || (ratio >= 0.75)) {
			delta = pnorm / 0.5;
			par *= 0.5;
		}
	}
	if (ratio >= 0.0001) {
		for (j = 0; j < n; j++) {
			//if (msk[j]) {           /* handle masked variables */
				params[j] = wa2[j];
			//}
			wa2[j] = diag[j] * params[j];
		}
		for (i = 0; i < m; i++)
			fvec[i] = wa4[i];
		xnorm = enorm(n,wa2);
		fnorm = fnorm1;
		iter++;
	}
	if ((fabs(actred) <= ftol) && (prered <= ftol) &&
		(0.5*ratio <= 1.0)) info = 1;
	if (delta <= xtol*xnorm) info = 2;
	if ((fabs(actred) <= ftol) && (prered <= ftol) &&
		(0.5*ratio <= 1.0) && (info == 2)) info = 3;
	if (nfev >= maxfev) info = 5;
	if ((fabs(actred) <= epsmch) && (prered <= epsmch) &&
		(0.5*ratio <= 1.0)) info = 6;
	if (delta <= epsmch*xnorm) info = 7;
	if (gnorm <= epsmch) info = 8;
	if (info != 0) {
		info = iflag;
		return;
	}
	if (ratio > 0.0001) LoopID = 0;
}

void CLMDif::Iter()
{
	if (info==0) {
		if (LoopID==0) {
			Loop0_();
			if (info==0) Loop1_();
		}
		else Loop1_();
	}
}

void CLMDif::fdjac2(CArray1D& x_, CArray1D& fvec_, CArray2D& fjac_,
	int& iflag_, double epsfcn_, CArray1D& wa_)
{
	int i,j;
	double eps,epsmch,h,temp;

	epsmch = (epsfcn_ > DPMPAR[0]) ? epsfcn_ : DPMPAR[0];
	eps = sqrt(epsmch);

	for (j = 0;j < n; j++) {
		temp = x_[j];
		if (temp == 0.0) h = eps;
                else h = eps * fabs(temp);
		x_[j] = temp + h;
		iflag_ = MinFunc(x_.getdata(),wa_.getdata(),cookie);
		if (iflag_ < 0) break;
		x_[j] = temp;
		for (i = 0;i < m; i++)
			fjac_[j][i] = (wa_[i] - fvec_[i]) / h;
	}
}

void CLMDif::CheckParams(CArray1D& apar)
{
	for(int i=0; i<n; i++) if (msk[i]==0) apar[i] = params_store[i];
}





