/**** NOTE: Modified 1-12-1999 to include parameter mask ****/
#ifndef MINPACK_H
#define MINPACK_H

const double DPMPAR[] = { 2.220446049250313e-16,
                    2.2250738585072014e-308,
                    1.7976931348623158e+308 };

typedef int (*LMDif_MinFunc)(double* x, double* y, void* cookie); 
/* type of function to be minimized by LMDif

		x	: the n parameters
		y	: the m equations
		cookie : pointer to pass parameters

  returns 
	positive if ok
	negative if an error occurs in the function evaluation
*/

class CLMDif 
{
/*
	Function fitting with modified levenberg-marquardt algorithm. from MINPACK
*/
public:
/* 
		n	: # of parameters
		m	: # of functions
		aFunc : a function that calculates the m functions
*/
	CLMDif(int an, int am, LMDif_MinFunc aFunc);

	virtual ~CLMDif() {}

	void SetCookie(void* acookie) { cookie = acookie; }

	// parameter rule implementation
	void CheckParams(CArray1D& aparams);
	void SetScales(CArray1D& ScaleFactors) { ScaleFactors = 1.; }

	/* driver routines */

	/* simple init */
	void Init(const CArray1D& ax, const CIntArray1D& amsk, double tol);

	/* detailed init */
	void Init(const CArray1D& ax, const CIntArray1D& amsk, 
		double aftol, double axtol,double agtol,
		int amaxfev, double aepsfcn, double afactor);

	/* iteration */
	void Iter();

	/* check for convergence */
	int Continue() { return info==0; }

	/* get info */

	/* calculate least square error = sqrt(Sum(yi-y(xi)^2)) */
	double GetError();

	/* get exit code */
	int GetExitCode() { return info; }

	/* get # of iterations */
	int GetIterCount() { return iter; }

	/* get # of function evaluations */
	int GetFuncEvals() { return nfev; }

	/* get the solution parameters */
	const CArray1D& GetSolution() { return params; }

protected:

	void Loop1_();
	void Loop0_();
	void LoopInit_();

	void initialize_();

	void fdjac2(CArray1D& x_, CArray1D& fvec_, CArray2D& fjac_,
	int& iflag_, double epsfcn_, CArray1D& wa_);

	int m,n;
	CArray2D fjac;
	CArray1D params,params_store,fvec,diag,qtf,wa1,wa2,wa3,wa4;
	CIntArray1D msk,ipvt;

	double ftol,xtol,gtol,epsfcn,factor;
	int mode,info,nfev,maxfev;
	double fnorm,epsmch,par;

	int iflag,iter;
    int i,j,l;
    double actred,delta,dirder,fnorm1,gnorm;
    double pnorm,prered,ratio,sum,temp,temp1,temp2,xnorm;

	int LoopID;

	LMDif_MinFunc MinFunc;
	void* cookie;
};

typedef int (*ModelFunc)(double* x, double* par, double* y); 
/* type of the model function for the fit

		y = y(x; par)

		x	: vector of x values; 
		y	: vector of y values
		par	: vector of parameters
  returns 
	positive if ok
	negative if an error occurs in the function evaluation
*/

class CLMFit : public CLMDif
{
protected:
	double* xdata; // ndata x nx matrix
	double* ydata; // ndata x ny matrix
	int nx,ny,ndata;
	ModelFunc Model;
public:
	CLMFit(ModelFunc aModel, int anpar, 
		int andata, int anx, int any, 
		double* axdata, double* aydata):
		CLMDif(anpar,andata*any,LMFitMinFunc),
		Model(aModel),
		xdata(axdata), ydata(aydata),
		nx(anx),ny(any),ndata(andata)
		{ SetCookie(this); }
	  
	virtual ~CLMFit() {}

	static int LMFitMinFunc(double* par, double* f, void* cookie)
	{
		return ((CLMFit*)cookie)->EvalMinFunc(par,f);
	}


	int Fit(CArray1D& ax, const CIntArray1D& amsk, double tol)
	{
		Init(ax,msk,1e-9);
		while (Continue()) Iter();
		ax = GetSolution();
		return GetExitCode();
	}

	void GetFit(double* par, double* f)
	{
		double* px = xdata;
		double* pf = f;
		double* pfend = pf + m*ny;

		while(pf<pfend)
		{
			Model(px,par,pf);
			pf += ny;
			px += nx;
		}
	}

protected:
	// function to be minimized by LMDif
	int EvalMinFunc(double* par, double* f)
	{
		//CheckParams(x);
		double* py = ydata;
		double* pyend = py + m*ny;
		double* px = xdata;
		double* pf = f;
		int flag;

		while(py<pyend && flag>0)
		{
			flag = Model(px,par,pf);
			for (int i=0; i<ny; i++) 
			{
				*(pf) = *(py++) - *(pf);
				pf++;
			}
			px += nx;
		}

		return flag;
	}
};

/* 
c
c     subroutine lmdif
c
c     the purpose of lmdif is to minimize the sum of the squares of
c     m nonlinear functions in n variables by a modification of
c     the levenberg-marquardt algorithm. the user must provide a
c     subroutine which calculates the functions. the jacobian is
c     then calculated by a forward-difference approximation.
c
c     the subroutine statement is
c
c       subroutine lmdif(fcn,m,n,x,fvec,ftol,xtol,gtol,maxfev,epsfcn,
c                        diag,mode,factor,nprint,info,nfev,fjac,
c                        ldfjac,ipvt,qtf,wa1,wa2,wa3,wa4)
c
c     where
c
c       fcn is the name of the user-supplied subroutine which
c         calculates the functions. fcn must be declared
c         in an external statement in the user calling
c         program, and should be written as follows.
c
c         subroutine fcn(m,n,x,fvec,iflag)
c         integer m,n,iflag
c         double precision x(n),fvec(m)
c         ----------
c         calculate the functions at x and
c         return this vector in fvec.
c         ----------
c         return
c         end
c
c         the value of iflag should not be changed by fcn unless
c         the user wants to terminate execution of lmdif.
c         in this case set iflag to a negative integer.
c
c       m is a positive integer input variable set to the number
c         of functions.
c
c       n is a positive integer input variable set to the number
c         of variables. n must not exceed m.
c
c       x is an array of length n. on input x must contain
c         an initial estimate of the solution vector. on output x
c         contains the final estimate of the solution vector.
c
c       fvec is an output array of length m which contains
c         the functions evaluated at the output x.
c
c       ftol is a nonnegative input variable. termination
c         occurs when both the actual and predicted relative
c         reductions in the sum of squares are at most ftol.
c         therefore, ftol measures the relative error desired
c         in the sum of squares.
c
c       xtol is a nonnegative input variable. termination
c         occurs when the relative error between two consecutive
c         iterates is at most xtol. therefore, xtol measures the
c         relative error desired in the approximate solution.
c
c       gtol is a nonnegative input variable. termination
c         occurs when the cosine of the angle between fvec and
c         any column of the jacobian is at most gtol in absolute
c         value. therefore, gtol measures the orthogonality
c         desired between the function vector and the columns
c         of the jacobian.
c
c       maxfev is a positive integer input variable. termination
c         occurs when the number of calls to fcn is at least
c         maxfev by the end of an iteration.
c
c       epsfcn is an input variable used in determining a suitable
c         step length for the forward-difference approximation. this
c         approximation assumes that the relative errors in the
c         functions are of the order of epsfcn. if epsfcn is less
c         than the machine precision, it is assumed that the relative
c         errors in the functions are of the order of the machine
c         precision.
c
c       diag is an array of length n. if mode = 1 (see
c         below), diag is internally set. if mode = 2, diag
c         must contain positive entries that serve as
c         multiplicative scale factors for the variables.
c
c       mode is an integer input variable. if mode = 1, the
c         variables will be scaled internally. if mode = 2,
c         the scaling is specified by the input diag. other
c         values of mode are equivalent to mode = 1.
c
c       factor is a positive input variable used in determining the
c         initial step bound. this bound is set to the product of
c         factor and the euclidean norm of diag*x if nonzero, or else
c         to factor itself. in most cases factor should lie in the
c         interval (.1,100.). 100. is a generally recommended value.
c
c       nprint is an integer input variable that enables controlled
c         printing of iterates if it is positive. in this case,
c         fcn is called with iflag = 0 at the beginning of the first
c         iteration and every nprint iterations thereafter and
c         immediately prior to return, with x and fvec available
c         for printing. if nprint is not positive, no special calls
c         of fcn with iflag = 0 are made.
c
c       info is an integer output variable. if the user has
c         terminated execution, info is set to the (negative)
c         value of iflag. see description of fcn. otherwise,
c         info is set as follows.
c
c         info = 0  improper input parameters.
c
c         info = 1  both actual and predicted relative reductions
c                   in the sum of squares are at most ftol.
c
c         info = 2  relative error between two consecutive iterates
c                   is at most xtol.
c
c         info = 3  conditions for info = 1 and info = 2 both hold.
c
c         info = 4  the cosine of the angle between fvec and any
c                   column of the jacobian is at most gtol in
c                   absolute value.
c
c         info = 5  number of calls to fcn has reached or
c                   exceeded maxfev.
c
c         info = 6  ftol is too small. no further reduction in
c                   the sum of squares is possible.
c
c         info = 7  xtol is too small. no further improvement in
c                   the approximate solution x is possible.
c
c         info = 8  gtol is too small. fvec is orthogonal to the
c                   columns of the jacobian to machine precision.
c
c       nfev is an integer output variable set to the number of
c         calls to fcn.
c
c       fjac is an output m by n array. the upper n by n submatrix
c         of fjac contains an upper triangular matrix r with
c         diagonal elements of nonincreasing magnitude such that
c
c                t     t           t
c               p *(jac *jac)*p = r *r,
c
c         where p is a permutation matrix and jac is the final
c         calculated jacobian. column j of p is column ipvt(j)
c         (see below) of the identity matrix. the lower trapezoidal
c         part of fjac contains information generated during
c         the computation of r.
c
c       ldfjac is a positive integer input variable not less than m
c         which specifies the leading dimension of the array fjac.
c
c       ipvt is an integer output array of length n. ipvt
c         defines a permutation matrix p such that jac*p = q*r,
c         where jac is the final calculated jacobian, q is
c         orthogonal (not stored), and r is upper triangular
c         with diagonal elements of nonincreasing magnitude.
c         column j of p is column ipvt(j) of the identity matrix.
c
c       qtf is an output array of length n which contains
c         the first n elements of the vector (q transpose)*fvec.
c
c       wa1, wa2, and wa3 are work arrays of length n.
c
c       wa4 is a work array of length m.
c
c     subprograms called
c
c       user-supplied ...... fcn
c
c       minpack-supplied ... dpmpar,enorm,fdjac2,lmpar,qrfac
c
c       fortran-supplied ... dabs,dmax1,dmin1,dsqrt,mod
c
c     argonne national laboratory. minpack project. march 1980.
c     burton s. garbow, kenneth e. hillstrom, jorge j. more
c
*/

#endif
