#include "stdafx.h"
#include "constants.h"
#include <math.h>

double FermiIntX[] = { -50.0, -5.0, -0.5, 0.5, 5., 50. };
double FermiIntValues[] = {	0.19287498479639E-21, 0.67219543145059E-02, 0.50753710355464,
1.1173314873128,8.8442088952430,266.09281252136};

void FD1_2(double X, double& F, double& dF);

void FermiFunc(double x, double g, double& f, double& df)
{
	if (x==0.) {
		df = 1 + g;
		f = g/df;
		df = -f/df;
	}
	else if (x<0) {
		double d = exp(x)/g;
		df = 1 + d;
		f = 1/df;
		df = -d*f*f;
	}
	else {
		double d = g*exp(-x);
		df = 1 + d;
		f = d/df;
		df = -f/df;
	}
}

void FermiIntegral(bool fermi, double x, double& f, double& df)
{
	if (fermi) FD1_2(x,f,df);
	else {
		f = exp(x); df = f;
	}
}

void ImpG(bool fermi, double x, double& G, double& dG, int g /* = 2 */)
/*
For computing the number of ionized impurities.
Returns

	1/(1+g*exp(-x))

and its derivative with respect to x.

g is the level degeneracy.
*/
{
	if (fermi) {
		if (x>=0) {
			double dd = exp(-x)*g, dd1 = 1+dd;
			G = 1/dd1;
			dG = dd/dd1/dd1;
		}
		else {
			double dd = exp(x)/g, dd1 = 1.+dd;
			G = dd/dd1;
			dG = G/dd1;
		}
	}
	else {
		G = 1.;
		dG = 0.;
	}
}


double CHEVAL(int N, double* A, double T)
/*
C   DESCRIPTION:
C
C      This function evaluates a Chebyshev series, using the
C      Clenshaw method with Reinsch modification, as analysed
C      in the paper by Oliver.
C
C
C   INPUT PARAMETERS
C
C       N - INTEGER - The no. of terms in the sequence
C
C       A - REAL ARRAY, dimension 0 to N - The coefficients of
C           the Chebyshev series
C
C       T - REAL - The value at which the series is to be
C           evaluated
C
C
C   REFERENCES
C
C        "An error analysis of the modified Clenshaw method for
C         evaluating Chebyshev and Fourier series" J. Oliver,
C         J.I.M.A., vol. 20, 1977, pp379-391
C
C
C   MACHINE-DEPENDENT CONSTANTS: NONE
C
C
C   INTRINSIC FUNCTIONS USED;
C
C      ABS
C
C
C    AUTHOR:  Dr. Allan J. MacLeod,
C             Dept. of Mathematics and Statistics,
C             University of Paisley ,
C             High St.,
C             PAISLEY,
C             SCOTLAND
C             ( e-mail:  macl-ms0@paisley.ac.uk )
C
C
C   LATEST MODIFICATION:
C                       21 September , 1995
C
*/
{
	int I;
	double D1,D2,TT,U0,U1=0.0,U2;
	//
	//   If ABS ( T )  < 0.6 use the standard Clenshaw method
	//
	if ( fabs( T ) < 0.6 ) {
		U0 = 0.0;
		TT = T + T;
		for( I = N; I>=0; I--) { 
            U2 = U1;
            U1 = U0;
            U0 = TT * U1 + A[ I ] - U2;
		}
		return ( U0 - U2 ) / 2.0;
	} else {
		//
		//   If ABS ( T )  > =  0.6 use the Reinsch modification
		//
		D1 = 0.0;
		//
		//   T > =  0.6 code
		//
		if ( T > 0.0 ) {
            TT =  ( T - 0.5 ) - 0.5;
            TT = TT + TT;
			for( I = N; I>=0; I--) { 
				D2 = D1;
				U2 = U1;
				D1 = TT * U2 + A[ I ] + D2;
				U1 = D1 + U2;
			}
            return  ( D1 + D2 ) / 2.0;
		} else {
			//
			//   T < =  -0.6 code
			//
            TT =  ( T + 0.5 ) + 0.5;
            TT = TT + TT;
			for( I = N; I>=0; I--) { 
				D2 = D1;
				U2 = U1;
				D1 = TT * U2 + A[ I ] - D2;
				U1 = D1 - U2;
			}
            return  ( D1 - D2 ) / 2.0;
		}
	}
}

void DIFF_CHEVAL(int N, double* A, double T, double& CHV, double& dCHV)
/*
C   DESCRIPTION:
C
C      This function evaluates a Chebyshev series, using the
C      Clenshaw method with Reinsch modification, as analysed
C      in the paper by Oliver.
C
C
C   INPUT PARAMETERS
C
C       N - INTEGER - The no. of terms in the sequence
C
C       A - REAL ARRAY, dimension 0 to N - The coefficients of
C           the Chebyshev series
C
C       T - REAL - The value at which the series is to be
C           evaluated
C
C
C   REFERENCES
C
C        "An error analysis of the modified Clenshaw method for
C         evaluating Chebyshev and Fourier series" J. Oliver,
C         J.I.M.A., vol. 20, 1977, pp379-391
C
C
C   MACHINE-DEPENDENT CONSTANTS: NONE
C
C
C   INTRINSIC FUNCTIONS USED;
C
C      ABS
C
C
C    AUTHOR:  Dr. Allan J. MacLeod,
C             Dept. of Mathematics and Statistics,
C             University of Paisley ,
C             High St.,
C             PAISLEY,
C             SCOTLAND
C             ( e-mail:  macl-ms0@paisley.ac.uk )
C
C
C   LATEST MODIFICATION:
C                       21 September , 1995
C
*/
{
	int I;
	double D1,D2,TT,U0,U1=0.0,U2;
	double dD1,dD2,dU0,dU1=0.0,dU2;
	//
	//   If ABS ( T )  < 0.6 use the standard Clenshaw method
	//
	if ( fabs( T ) < 0.6 ) {
		U0 = 0.0;
		dU0 = 0.0;
		TT = T + T;
		for( I = N; I>=0; I--) { 
            U2 = U1;
            U1 = U0;
            U0 = TT * U1 + A[ I ] - U2;
            dU2 = dU1;
            dU1 = dU0;
            dU0 = U1 + TT*dU1 - dU2;
		}
		CHV = (U0 - U2) / 2.0;
		dCHV = dU0 - dU2;
	} else {
		//
		//   If ABS ( T )  > =  0.6 use the Reinsch modification
		//
		D1 = 0.0;
		dD1 = 0.0;
		//
		//   T > =  0.6 code
		//
		if ( T > 0.0 ) {
            TT =  ( T - 0.5 ) - 0.5;
            TT = TT + TT;
			for( I = N; I>=0; I--) { 
				D2 = D1;
				U2 = U1;
				D1 = TT * U2 + A[ I ] + D2;
				U1 = D1 + U2;
				dD2 = dD1;
				dU2 = dU1;
				dD1 = U2 + TT * dU2 + dD2;
				dU1 = dD1 + dU2;
			}
			CHV = ( D1 + D2 ) / 2.0;
            dCHV = ( dD1 + dD2 );
		} else {
			//
			//   T < =  -0.6 code
			//
            TT =  ( T + 0.5 ) + 0.5;
            TT = TT + TT;
			for( I = N; I>=0; I--) { 
				D2 = D1;
				U2 = U1;
				D1 = TT * U2 + A[ I ] - D2;
				U1 = D1 - U2;
				dD2 = dD1;
				dU2 = dU1;
				dD1 = U2 + TT * dU2 - dD2;
				dU1 = dD1 - dU2;
			}
			CHV = ( D1 - D2 ) / 2.0;
            dCHV = ( dD1 - dD2 );
		}
	}
}

#define GAM2P5	0.1329340388179137E1
#define TWOE	5.4365636569180905E0
//
//   Machine-dependent constants (suitable for IEEE machines)
//
#define NTERM1	13
#define NTERM2	23
#define NTERM3	53
#define XMIN1	-35.7
#define XMIN2	-708.394
#define XHIGH1	7.45467E7
#define XHIGH2	3.8392996E205


void FD1_2(double X, double& F, double& dF)
/*
C   DESCRIPTION:
C
C      This function computes the Fermi-Dirac function of
C      order 1/2, defined as
C
C                     Int{0 to inf} t**(1/2) / (1+exp(t-x)) dt
C         F(x)     = -----------------------------------------
C                                 Gamma(3/2)
C
C      The function uses Chebyshev expansions which are given to
C      16 decimal places for x <= 2, but only 10 decimal places 
C      for x > 2.
C
C
C   ERROR RETURNS:
C    
C      If XVALUE too large and positive, the function value
C      will overflow. An error message is printed and the function
C      returns the value 0.0.
C
C
C   MACHINE-DEPENDENT CONSTANTS:
C
C      NTERMS1 - INTEGER - The number of terms used from the array
C                          ARRFD1. The recommended value is such that
C                               ABS(ARRFD1(NTERMS1)) < EPS/10
C                          subject to 1 <= NTERMS1 <= 13.
C
C      NTERMS2 - INTEGER - The number of terms used from the array
C                          ARRFD2. The recommended value is such that
C                               ABS(ARRFD2(NTERMS2)) < EPS/10
C                          subject to 1 <= NTERMS1 <= 23.
C
C      NTERMS3 - INTEGER - The number of terms used from the array
C                          ARRFD3. The recommended value is such that
C                               ABS(ARRFD3(NTERMS3)) < EPS/10
C                          subject to 1 <= NTERMS3 <= 32.
C
C      XMIN1 - REAL - The value of x below which
C                         FDP0P5(x) = exp(x)
C                     to machine precision. The recommended value
C                     is   1.5*LN(2) + LN(EPSNEG)
C
C      XMIN2 - REAL - The value of x below which
C                         FDP0P5(x) = 0.0 
C                     to machine precision. The recommended value
C                     is    LN ( XMIN )
C
C      XHIGH1 - REAL - The value of x above which
C                         FDP0P5(x) = x**(3/2)/GAMMA(5/2)
C                     to machine precision. The recommended value
C                     is   pi / SQRT(8*EPS)
C
C      XHIGH2 - REAL - The value of x above which FDP0P5 would 
C                      overflow. The reommended value is
C                              (1.329*XMAX)**(2/3)
C
C      For values of EPS, EPSNEG, and XMIN the user should refer to the
C      paper by Cody in ACM. Trans. Math. Soft. Vol. 14 (1988) p303-311.
C   
C      This code is provided with single and double precision values
C      of the machine-dependent parameters, suitable for machines
C      which satisfy the IEEE floating-point standard.
C
C
C   AUTHOR:
C          DR. ALLAN MACLEOD,
C          DEPT. OF MATHEMATICS AND STATISTICS,
C          UNIVERSITY OF PAISLEY,
C          HIGH ST.,
C          PAISLEY,
C          SCOTLAND
C          PA1 2BE
C
C          (e-mail: macl-ms0@paisley.ac.uk )
C
C
C   LATEST UPDATE:
C                 20 NOVEMBER, 1996
*/
{
	static double ARRFD1[] = { 
		1.8862968392734597e0,
		-0.543580817644053E-1,
		0.23644975439720E-2,
		-0.1216929365880E-3,
		0.68695130622E-5,
		-0.4112076172E-6,
		0.256351628E-7,
		-0.16465008E-8,
		0.1081948E-9,
		-0.72392E-11,
		0.4915E-12,
		-0.338E-13,
		0.23E-14,
		-0.2E-15
	};
	static double ARRFD2[] = { 
		2.6982492788170612E0,
       1.2389914141133012E0,
       0.2291439379816278E0,
       0.90316534687279E-2,
      -0.25776524691246E-2,
      -0.583681605388E-4,
       0.693609458725E-4,
      -0.18061670265E-5,
      -0.21321530005E-5,
       0.1754983951E-6,
       0.665325470E-7,
      -0.101675977E-7,
      -0.19637597E-8,
       0.5075769E-9,
       0.491469E-10,
      -0.233737E-10,
      -0.6645E-12,
       0.10115E-11,
      -0.313E-13,
      -0.412E-13,
       0.38E-14,
       0.16E-14,
      -0.3E-15,
      -0.1E-15
	};


	static double ARRFD3[] = {
	 2.5484384198009122E0,
     0.510439408960652E-1,
     0.77493527628294E-2,
    -0.75041656584953E-2,
    -0.77540826320296E-2,
    -0.45810844539977E-2,
    -0.23431641587363E-2,
    -0.11788049513591E-2,
    -0.5802739359702E-3,
    -0.2825350700537E-3,
    -0.1388136651799E-3,
    -0.680695084875E-4,
    -0.335356350608E-4,
    -0.166533018734E-4,
    -0.82714908266E-5,
    -0.41425714409E-5,
    -0.20805255294E-5,
    -0.10479767478E-5,
    -0.5315273802E-6,
    -0.2694061178E-6,
    -0.1374878749E-6,
    -0.702308887E-7,
    -0.359543942E-7,
    -0.185106126E-7,
    -0.95023937E-8,
    -0.49184811E-8,
    -0.25371950E-8,
    -0.13151532E-8,
    -0.6835168E-9,
    -0.3538244E-9,
    -0.1853182E-9,
    -0.958983E-10,
    -0.504083E-10,
    -0.262238E-10,
    -0.137255E-10,
    -0.72340E-11,
    -0.37429E-11,
    -0.20059E-11,
    -0.10269E-11,
    -0.5551E-12,
    -0.2857E-12,
    -0.1520E-12,
    -0.811E-13,
    -0.410E-13,
    -0.234E-13,
    -0.110E-13,
    -0.67E-14,
    -0.30E-14,
    -0.19E-14,
    -0.9E-15,
    -0.5E-15,
    -0.3E-15,
    -0.1E-15,
    -0.1E-15
	};

//
//   Start calculation
//
      double CHV,dCHV;
//
//   Test for error condition
//
      if ( X > XHIGH2 ) { F=0.; dF=0.; }
//
//   Code for x < -1
//
      if ( X < -1. ) {
		  if ( X > XMIN1 ) {
            double EXPX = exp(X);
            double T = TWOE * EXPX - 1.0;
			DIFF_CHEVAL ( NTERM1 , ARRFD1 , T, CHV, dCHV );
			F = EXPX * CHV;
            dF = F + TWOE*EXPX*EXPX*dCHV;			 
		  } else {
			  if ( X < XMIN2 ) { F=0.; dF=0.; }
			  else { F = exp(X); dF = F; }
		  }
      } else if ( X <= 2.0 ) {
//
//   Code for -1 <= x <= 2
//
            double T = ( 2.0 * X - 1.0 ) / 3.0;
			DIFF_CHEVAL ( NTERM2 , ARRFD2 , T, F, dCHV );
			dF = 0.6666666666666666666666 * dCHV;
	  } else {
//
//   Code for x > 2
//
            F = sqrt(X) / GAM2P5;
			dF = 1.5*F;
            F = X * F;
            if ( X <= XHIGH1 ) { 
               double XSQ = X * X;
			   double dd = 42 + XSQ;
               double T = ( 50.0  - XSQ ) / dd;
			   DIFF_CHEVAL ( NTERM3 , ARRFD3 , T, CHV, dCHV );
			   double cc = 1.0 + CHV / XSQ;
			   dF = dF*cc + F*( -2.0*CHV/XSQ/X - 184.0*dCHV/X/dd/dd );
			   F *= cc;
			}
	  }
}


float SiliconCP(float Nd, float Na, float beta, bool fermi)
{
	// initial estimate
	float Nc = (float)SI_NC*beta/N0;
	float Nv = (float)SI_NV*beta/N0;
	Nd = (float)(beta*Nd/N0);
	Na = (float)(beta*Na/N0);
	float Ec = 0.56*beta;
	float Ev = -0.56*beta;

	double v = (Nd-Na)/sqrt(Nc*Nv)/exp(-(Ec - Ev)/2)/2;
	v = asinh(v);
	v = 0.5*log(Nv/Nc) + v;
	v = (Ec - Ev)/2 - Ec - v;
	
	// refine
	double q,dq;
	do {
		{
			double ec = Ec+v, ev = Ev+v;
			double n,dn,p,dp;
			
			ImpG(fermi,ec-beta*0.04,n,dn,2);
			ImpG(fermi,-ev-beta*0.04,p,dp,4);
			q = Nd*n - Na*p;
			dq = Nd*dn + Na*dp;
			
			FermiIntegral(fermi,-ec,n,dn);
			q -= Nc*n; 
			dq += Nc*dn; 
			
			FermiIntegral(fermi,ev,p,dp);
			q += Nv*p; 
			dq += Nv*dp; 
		} 
		v -= q/dq;
	} while (fabs(q)>1e-6);
	
	return v/beta;
}

float FindSemiconductorCP(float Ec, float Ev, float Nc, float Nv,
						  float Nd, float Na, float beta, bool fermi)
{
	// initial estimate
	Nc = (float)(Nc*beta/N0);
	Nv = (float)(Nv*beta/N0);
	Nd = (float)(beta*Nd/N0);
	Na = (float)(beta*Na/N0);
	Ec = Ec*beta;
	Ev = Ev*beta;

	double v = (Nd-Na)/sqrt(Nc*Nv)/exp(-(Ec - Ev)/2)/2;
	v = asinh(v);
	v = 0.5*log(Nv/Nc) + v;
	v = (Ec - Ev)/2 - Ec - v;
	
	// refine
	double q,dq;
	do {
		{
			double ec = Ec+v, ev = Ev+v;
			double n,dn,p,dp;
			
			ImpG(fermi,ec-beta*0.04,n,dn,2);
			ImpG(fermi,-ev-beta*0.04,p,dp,4);
			q = Nd*n - Na*p;
			dq = Nd*dn + Na*dp;
			
			FermiIntegral(fermi,-ec,n,dn);
			q -= Nc*n; 
			dq += Nc*dn; 
			
			FermiIntegral(fermi,ev,p,dp);
			q += Nv*p; 
			dq += Nv*dp; 
		} 
		v -= q/dq;
	} while (fabs(q)>1e-6);
	
	return v/beta;
}