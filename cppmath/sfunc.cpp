#include "cppmath.h"
#include <math.h>
#include <limits>

double asinh(double z)
{
	int k = 1;
	if (z<0) { z=-z; k=-1;}
	return k*log(z + sqrt(z*z+1));
}

double erfcore(double x, int jint);

double Erf(double x) { return erfcore(x,0); }
double Erfc(double x) { return erfcore(x,1); }
double Erfcx(double x) { return erfcore(x,2); }


static double erfa[] = {3.16112374387056560e00, 1.13864154151050156e02,
             3.77485237685302021e02, 3.20937758913846947e03,
             1.85777706184603153e-1};
static double erfb[] = {2.36012909523441209e01, 2.44024637934444173e02,
             1.28261652607737228e03, 2.84423683343917062e03};
static double erfc[] = {5.64188496988670089e-1, 8.88314979438837594e00,
             6.61191906371416295e01, 2.98635138197400131e02,
             8.81952221241769090e02, 1.71204761263407058e03,
             2.05107837782607147e03, 1.23033935479799725e03,
             2.15311535474403846e-8};
static double erfd[] = {1.57449261107098347e01, 1.17693950891312499e02,
             5.37181101862009858e02, 1.62138957456669019e03,
             3.29079923573345963e03, 4.36261909014324716e03,
             3.43936767414372164e03, 1.23033935480374942e03};
static double erfp[] = {3.05326634961232344e-1, 3.60344899949804439e-1,
             1.25781726111229246e-1, 1.60837851487422766e-2,
             6.58749161529837803e-4, 1.63153871373020978e-2};
static double erfq[] = {2.56852019228982242e00, 1.87295284992346047e00,
             5.27905102951428412e-1, 6.05183413124413191e-2,
             2.33520497626869185e-3};

double erfcore(double x, int jint)
/*
%ERFCORE Core algorithm for error functions.
%   erf(x) = erfcore(x,0)
%   erfc(x) = erfcore(x,1)
%   erfcx(x) = exp(x^2)*erfc(x) = erfcore(x,2)

%   C. Moler, 2-1-91.
%   Copyright 1984-2000 The MathWorks, Inc. 
%   $Revision: 5.12 $  $Date: 2000/07/28 18:59:49 $

%   This is a translation of a FORTRAN program by W. J. Cody,
%   Argonne National Laboratory, NETLIB/SPECFUN, March 19, 1990.
%   The main computation evaluates near-minimax approximations
%   from "Rational Chebyshev approximations for the error function"
%   by W. J. Cody, Math. Comp., 1969, PP. 631-638.
%#mex
*/
{
//
//   evaluate  erf  for  |x| <= 0.46875
//
	double ret,y=fabs(x),xnum,xden;
	if (y<=0.46875) {
            double z = y * y;
            xnum = erfa[4]*z;
            xden = z;
            for(int i = 0; i<3; i++) {
               xnum = (xnum + erfa[i]) * z;
               xden = (xden + erfb[i]) * z;
            }
            ret = x * (xnum + erfa[3]) / (xden + erfb[3]);
            if (jint != 0) ret = 1 - ret; 
            if (jint == 2) ret = exp(z) * ret;
    }
//
//   evaluate  erfc  for 0.46875 <= |x| <= 4.0
//
    else if (y<=4.0) {
            xnum = erfc[8]*y;
            xden = y;
            for(int i = 0; i<7; i++) {
               xnum = (xnum + erfc[i]) * y;
               xden = (xden + erfd[i]) * y;
            }
            ret = (xnum + erfc[7]) / (xden + erfd[7]);
            if (jint != 2) {
               double z = floor(y*16)/16;
               double del = (y-z)*(y+z);
               ret = exp(-z*z) * exp(-del) * ret;
			}
	}
//
//   evaluate  erfc  for |x| > 4.0
//
    else {
            double z = 1 / (y * y);
            xnum = erfp[5]*z;
            xden = z;
            for(int i = 0; i<4; i++) {
               xnum = (xnum + erfp[i]) * z;
               xden = (xden + erfq[i]) * z;
            }
            ret = z * (xnum + erfp[4]) / (xden + erfq[4]);
            ret = (1/sqrt(PI) -  ret) / y;
            if (jint != 2) {
               z = floor(y*16)/16;
               double del = (y-z)*(y+z);
               ret = exp(-z*z) * exp(-del) * ret;
               
		}
	}
//
//   fix up for negative argument, erf, etc.
//
    if (jint == 0) {
		if (x>0.46875) ret = (0.5-ret)+0.5;
		if (x<-0.46875) ret = (-0.5+ret)-0.5;
	}
    else if (jint == 1) {
		if (x<-0.46875) ret = 2. - ret;
	}
    else  { // jint must = 2 
			if (x<-0.46875) {
				double z = floor(y*16)/16;
				if (x<0) z = -z; 
				double del = (x-z)*(x+z);
				y = exp(z*z) * exp(del);
				ret = (y+y) - ret;
			}
	}

	return ret;
}