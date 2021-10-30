#ifndef CPPMATH_H
#define CPPMATH_H

#pragma warning( disable : 4275 )  // Disable warning messages

#define CPPMATH_OK				0
#define CPPMATH_ERR_LU_SING		1
#define CPPMATH_ERR_INTERP		2
#define CPPMATH_ERR_ITER		3
#define CPPMATH_ERR_SING		4

#include "tnt110/tnt.h"

#include "stopwatch.h"

#include "tridiag.h"
#include "tridiagsym.h"


#include <complex>

#define PI 3.1415926535897932384626433832795

typedef std::complex<double> cdouble;
typedef std::complex<float> cfloat;

typedef TNT::Array1D<double>	CArray1D;
typedef TNT::Array1D<cdouble>	CComplexArray1D;
typedef TNT::Array1D<int>		CIntArray1D;

typedef TNT::Array2D<double>	CArray2D;
typedef TNT::Array2D<cdouble>	CComplexArray2D;
typedef TNT::Array2D<int>		CIntArray2D;

typedef	Tridiag_Array2D<double>	CTridiag2D;
typedef	TridiagSym_Array2D<double>	CTridiagSym2D;
typedef	Tridiag_Array2D<cdouble>	CComplexTridiag2D;
typedef	TridiagSym_Array2D<cdouble>	CComplexTridiagSym2D;


#include "cppminpack.h"
#include "ode.h"
#include "ptable.h"
#include "filters.h"
#include "marsaglia.h"
#include "utils.h"

int textload(const char* fname, CArray1D& A);
int textload(const char* fname, int nCol, CArray2D& A, int trans=0);
int textsave(const char* fname, const CArray1D& A);
int textsave(const char* fname, const CArray2D& A);
int textsave(ostream& os, const CArray1D& A);
int textsave(ostream& os, const CArray2D& A);

template<class T>
int textload(istream& is, TNT::Array1D<T>& A);


//Special functions
double asinh(double z);
double Erf(double x);
double Erfc(double x);
double Erfcx(double x);

//utility functions
double sign(double x);
double adj_int(double x);
double intdiv(double x, double y);

//Linear Algebra
/* LU */
int Ludcmp(TNT::Array2D<float>& A, CIntArray1D& indx, int& d);
int Ludcmp(TNT::Array2D<cfloat>& A, CIntArray1D& indx, int& d);
int Ludcmp(CArray2D& A, CIntArray1D& indx, int& d);
int Ludcmp(CComplexArray2D& A, CIntArray1D& indx, int& d);

void Lubksb(TNT::Array2D<float>& A, CIntArray1D& indx, TNT::Array1D<float>& b);
void Lubksb(TNT::Array2D<cfloat>& A, CIntArray1D& indx, TNT::Array1D<cfloat>& b);
void Lubksb(CArray2D& A, CIntArray1D& indx, CArray1D& b);
void Lubksb(CComplexArray2D& A, CIntArray1D& indx, CComplexArray1D& b);

/* solve tridiagonal linear system */
int Dgtsl(CTridiag2D& T, CArray1D& b);
int Dgtsl(CComplexTridiag2D& T, CComplexArray1D& b);
int Dgtsl(Tridiag_Array2D<float>& T, TNT::Array1D<float>& b);
int Dgtsl(Tridiag_Array2D<cfloat>& T, TNT::Array1D<cfloat>& b);
	
void Dptsl(CTridiagSym2D& T, CArray1D& b);

void invM(int n, CArray2D& W);
	
class CGridRefiner
{
public:
	CGridRefiner(int an);
	CGridRefiner() {}
	void Init(int an);
	bool Refine(CArray1D& x, CArray1D& y, double alpha = 0.5);
	bool Refine(double* x, double* y, double alpha = 0.5);
	void Convert(double* z);
	void Convert(CArray1D& z);
protected:
	void CalcF_(double* x, double* y);
	bool CalcX1_(double delta, double* x);
	CArray2D W;
	CArray1D df,f,x1,u,dx;
	CArray1D newx,oldx,F;
	int n2,n;
};

	
// eigenvalues
int Imtql2(const CTridiagSym2D& T, CArray1D& W, CArray2D& Z, CArray1D& g);
int Tsturm(TridiagSym_Array2D<double>& T, TNT::Array1D<double>& W, TNT::Array2D<double>& Z, 
		   double lb, double ub, int& m, TNT::Array2D<double>& g);
int Tsturm(TridiagSym_Array2D<float>& T, TNT::Array1D<float>& W, TNT::Array2D<float>& Z, 
		   float lb, float ub, int& m, TNT::Array2D<float>& g);


//Calculus
CArray1D savgol(int nl, int nr, int ld, int m);
CArray1D savgolfilt(const CArray1D& x, int nlr,  int m, int ld, double dx = 1.);

//Interpolation
int polint(double* xac, double* yac, int n, double x, double& y, double& dy);

//FFT
CArray1D fft_pow(const CArray1D& A);
double FindDominantFrequency (const CArray1D& ampl, double samplingRate, int windowSize, double cutoff=20.);
void four1(double datain[], unsigned long nn, int isign);
void realft(double datain[], unsigned long n, int isign);


void imfft(const CArray2D& Image, CComplexArray2D& FFT);

// Arrays as bitmaps
void bmpwrite(const CArray2D& A, const char *bmpfile);
void bmpread(const char *bmpfile, CArray2D& A);

//Integration of Functions
typedef double (*INTEG_FUNC)(double, void* ); 
double midpnt(INTEG_FUNC func, double a, double b, int n, void* par = 0);
double qromo(INTEG_FUNC func, double a, double b,
			double (*choose)(INTEG_FUNC, double, double, int, void*), 
			double EPS = 1.e-6, void* par = 0);
double qromb(INTEG_FUNC func, double a, double b,
			 double EPS = 1.e-6, void* par  = NULL );
void gauleg(double x1, double x2, CArray1D& x_c, CArray1D& w_c, int n);

template<class T>
class CSpliner {
public:
	CSpliner(TNT::Array1D<T>& ax, TNT::Array1D<T>& ay, int isnatural = 1, T yp1 = 0, T ypn = 0);
	CSpliner(int n, T* ax, T* ay, int isnatural = 1, T yp1 = 0, T ypn = 0);
	T Spline(T xi);
	T operator()(T xi) { return Spline(xi); }
protected:
	void InitSpline(int isnatural, T yp1, T ypn);
	TNT::Array1D<T> x,y,y2;
};

template<class T>
class CSpliner2D {
public:
	//CSpliner2D(TNT::Array1D<T>& ax1, TNT::Array1D<T>& ax2, TNT::Array2D<T>& ay);
	CSpliner2D(int n, int m, T* ax1, T* ax2, T* ay);
	~CSpliner2D();
	T Spline(T xi1, T xi2);
	T operator()(T xi1, T xi2) { return Spline(xi1,xi2); }
protected:
	TNT::Array1D<T> x1,x2,y1;
	TNT::Array1D<CSpliner<T>*> Spliners1D;
};

template <class T>
class CBiCuInt
{
public:
	CBiCuInt(int n, int m,
		T* aX1, T* aX2, T* aY, T* aY1, T* aY2, T* aY12):
	X1(n,aX1),X2(m,aX2),Y(n,m,aY),Y1(n,m,aY1),Y2(n,m,aY2),Y12(n,m,aY12),
		y_(4),y1_(4),y2_(4),y12_(4),
		cl_(16),xl_(16),
		c_(4,4)
	{}

	void Interp(T x1, T x2, T& y, T& y1, T& y2);
		 
protected:
	void GetCoeff();
	void GetInterp(T& ansy, T& ansy1, T& ansy2);
	int Locate(T x, TNT::Array1D<T>& X, T& d, T&u);

	T d1,d2,t,u;
	
	TNT::Array1D<T> X1,X2;
	TNT::Array2D<T> Y,Y1,Y2,Y12;
	TNT::Array1D<T> y_,y1_,y2_,y12_,cl_,xl_;
	TNT::Array2D<T> c_;
};

#endif