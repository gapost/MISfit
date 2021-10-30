#ifndef FUNCS_H
#define FUNCS_H

#include "cppminpack.h"

double enorm(int n, CArray1D& a);
double rownorm(int m, int r, int c, CArray2D& a);
double colnorm(int m, int r, int c, CArray2D& a);

/*void fdjac2(LMDIF_FUNC f,int m, int n,
			CArray1D& x,CArray1D& fvec,CArray2D& fjac,
	int *iflag, double epsfcn, CArray1D& wa,void* pParam);
*/
void lmpar(int n, CArray2D& r, CIntArray1D& ipvt,CArray1D& diag,
    CArray1D& qtb,double delta,double *par, CArray1D& x, CArray1D& sdiag,
    CArray1D& wa1,CArray1D& wa2);

void qrfac(int m,int n,CArray2D& a,int pivot,CIntArray1D& ipvt,
    CArray1D&  rdiag,CArray1D&  acnorm,CArray1D&  wa);
/*
void lmdif(LMDIF_FUNC f,int m,int n,CArray1D& x,CIntArray1D& msk,CArray1D& fvec,
    double ftol,double xtol,double gtol,int maxfev,double epsfcn,
    CArray1D& diag,int mode,double factor,int *info,int *nfev,
    CArray2D& fjac,CIntArray1D& ipvt,CArray1D& qtf,CArray1D& wa1,CArray1D& wa2,
    CArray1D& wa3,CArray1D& wa4,void* pParam);
*/
void qrsolv(int n,CArray2D& r,CIntArray1D& ipvt,CArray1D& diag,
    CArray1D& qtb,CArray1D& x,CArray1D& sdiag,CArray1D& wa);

#endif