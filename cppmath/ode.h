#ifndef ODE_H
#define ODE_H
/**** ODE Integrators *****/

// Runge Kutta ODE Integrators

// 4th order
template<class T>
class CRungeKutta
{
public:
	CRungeKutta(int anvar) { Init(anvar); }
	virtual ~CRungeKutta() {}
	int Integrate(TNT::Array1D<T>& vstart, T x1, T x2, int nstep);
	int Integrate(TNT::Array1D<T>& vstart, T x1, T x2, int nstep, TNT::Array1D<T>& xx, TNT::Array2D<T>& y);
	virtual void odefunc(T x, const TNT::Array1D<T>& y, TNT::Array1D<T>& dydx) {}
	void reset() { if (cerror>0) cerror = 0; }
protected:
	TNT::Array1D<T> vout,dv;
	TNT::Array1D<T> dym,dyt,yt;

	int nvar;
	int cerror;

	int Init(int anvar);
	void rk4(TNT::Array1D<T>& y, TNT::Array1D<T>& dydx, 
			 T x, T h, TNT::Array1D<T>& yout);

};

// Adaptive step
template<class T>
class CAdptRungeKutta
{
public:
	CAdptRungeKutta(int anvar) { Init(anvar); }
	virtual ~CAdptRungeKutta() {}
	int Integrate(TNT::Array1D<T>& ystart, 
				T x1, T x2, 
				T eps, T h1, T hmin,
				int& kmax, TNT::Array1D<T>& xp, TNT::Array2D<T>& yp, T dxsav);
	int Integrate(TNT::Array1D<T>& ystart, 
				T x1, T x2, 
				T eps, T h1, T hmin);
	virtual void odefunc(T x, const TNT::Array1D<T>& y, TNT::Array1D<T>& dydx) {}
	void reset() { if (cerror>0) cerror = 0; }
protected:
	TNT::Array1D<T> yscal,y,dydx;
	TNT::Array1D<T> yerr,ytemp,ak2,ak3,ak4,ak5,ak6;

	int nvar, nok, nbad, kount;
	int cerror;

	int Init(int anvar);
	void rkck(T x, T h);
	int rkqs(T *x, T htry, T eps,
		  T *hdid, T *hnext);

};

// Burlisch-Stoer integrator

template<class T>
class CBulirschStoer
/*
Integrate a system of 1st order diff. equations

errors:
0	no error
1	step size underflow
2   step size too small
3	too many iterations
*/
{
public:
	CBulirschStoer(int anvar);
	virtual ~CBulirschStoer() {}
	int Integrate(TNT::Array1D<T>& ystart, 
				T x1, T x2, 
				T eps, T h1, T hmin,
				int& kmax, TNT::Array1D<T>& xp, TNT::Array2D<T>& yp, T dxsav);
	int Integrate(TNT::Array1D<T>& ystart, 
				T x1, T x2, 
				T eps, T h1, T hmin);
	virtual void odefunc(T x, const TNT::Fortran_Array1D<T>& y, TNT::Fortran_Array1D<T>& dydx) {}
	void reset() { if (cerror>0) cerror = 0; }
protected:
	TNT::Fortran_Array1D<T> y,dydx,yscal,yerr,ysav,yseq,ytemp,ytemp2;
	TNT::Fortran_Array1D<T> x_,err_;
	TNT::Fortran_Array2D<T> d_;
	TNT::Fortran_Array1D<T> a;
	TNT::Fortran_Array2D<T> alf;
	TNT::Fortran_Array1D<T> nseq;

	int nvar, // Number of vars
		nok, nbad, kount;
	int cerror;
	// variables used by bsstep
	int first,kmax,kopt;
	T epsold,xnew;

	int Init(int anvar);
	void bsstep(T *xx, T htry, T eps, T *hdid, T *hnext);
	void pzextr(int iest, T xest, TNT::Fortran_Array1D<T>& yest, TNT::Fortran_Array1D<T>& yz, TNT::Fortran_Array1D<T>& dy);
	virtual void mmid(TNT::Fortran_Array1D<T>& y,	
		TNT::Fortran_Array1D<T>& d2y, T xs, T htot, int nstep, TNT::Fortran_Array1D<T>& yout);
};


// Stoermer's rule integrator

template<class T>
class CStoermer : public CBulirschStoer<T>
{
public:
	CStoermer(int aneqns) : neqns(aneqns), CBulirschStoer<T>(2*aneqns) {};
	virtual ~CStoermer() {}
protected:
	int neqns; // Number of equations.
	virtual void mmid(TNT::Fortran_Array1D<T>& y,
		TNT::Fortran_Array1D<T>& d2y, T xs, T htot, int nstep, TNT::Fortran_Array1D<T>& yout);
};

// 2 point Boundary Value Problem
// Relaxation
class CBVRelax {
public:
	CBVRelax(int ane, int am, int anb);
	virtual ~CBVRelax() {}

	int Solve(int itmax, double conv, double slowc, TNT::Fortran_Array2D<double>& y);
	virtual void Difeq(int k, int k1, int k2, int jsf, int is1, int isf,
					 TNT::Fortran_Array2D<double>& y) {}

	int ne,m,nb;
	TNT::Fortran_Array1D<double> scalv;
	TNT::Fortran_Array1D<int> indexv;
	double err;
protected:
	TNT::Fortran_Array2D<double> s;
	TNT::Fortran_Array3D<double> c;
	TNT::Fortran_Array1D<int> kmax,ermax;

	void bksub(int ne, int nb, int jf, int k1, int k2);
	int pinvs(int ie1, int ie2, int je1, int jsf, int jc1, int k);
	void red(int iz1, int iz2, int jz1, int jz2, int jm1, int jm2, int jmf,
		 int ic1, int jc1, int jcf, int kc);
};

#endif