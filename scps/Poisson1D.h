// Poisson1D.h: interface for the CPoisson1D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POISSON1D_H__6933662D_63CF_41CE_977C_41A8B0661205__INCLUDED_)
#define AFX_POISSON1D_H__6933662D_63CF_41CE_977C_41A8B0661205__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Poisson1DItem.h"

#include "Schrodinger1D.h"

class CPoisson1D 
{
public:
	CPoisson1D(double T=300) : beta(300/T/KT300),
		First(0), Last(0), nItem(0)
	{}
	virtual ~CPoisson1D();

	void Add(CPoisson1DItem* P);

	virtual int Init(double xo = 0.);
	void InitSolve(double aeps = 1e-5, double aconv = 1.0, int aitmax = 200)
	{ eps = aeps; conv = aconv; itmax = aitmax; err = eps + 1.; itcount=0; }
	virtual double Iter(int hf=0);
	bool Continue();
	int GetIter() { return itcount; }
	double GetErr() { return err; }
	double Solve(double aeps = 1e-5, double aconv = 1.0, int aitmax = 200, int hf=0);
	//double SolveRefine(double delta = 0.5, double eps = 1e-6, double aconv = 1.0, int itmax = 200);

	void Dump(const char* s);

	double GetTotalQ();
	double CalcClf()   { return CalcC_(0); }
	double CalcChf()   { return CalcC_(1); }
	virtual cdouble CalcC(double omega);
	double CalcTotalQ();
	
	int GetM() { return M; }

	CPoisson1DItem* GetFirst() { return First; }
	CPoisson1DItem* GetLast() { return Last; }

	CArray1D x,h,V,CP,F; 
	CArray2D Q,dQ;
protected:
	void Dump(ostream &s);
	virtual double CalcC_(int hf);

	double beta;
	double err,conv,eps,itmax,itcount;
	int M;
	CTridiagSym2D T,A;
	CTridiag2D C;
	CArray1D b;

	CComplexArray1D cb;
	CComplexTridiag2D cC;

	CPoisson1DItem *First,*Last;
	int nItem;

	friend class CPoisson1DItem;
};

class CSCPS : public CPoisson1D 
{
public:
	CSCPS(double T=300.) : 
	  CPoisson1D(T),
	  Electrons(1,T),
	  Holes(-1,T)
	  {}

	CMultiBandSchrodinger1D& GetElectrons() { return Electrons; }
	CMultiBandSchrodinger1D& GetHoles() { return Holes; }

	virtual int Init(double xo = 0.);
	virtual double Iter(int hf=0);
	virtual cdouble CalcC(double freq);

	double CalcI() { return Electrons.CalcI() + Holes.CalcI(); }

protected:
	CArray1D Qe, dQe, Qh, dQh, L;
	CArray2D DeltaE,DeltaH,Delta,DC;
	CIntArray1D Idx;
	CComplexArray2D cDC;

	int Ms, iQstart;

	CMultiBandSchrodinger1D Electrons, Holes;

	double CalcC_(int hf=0);

	friend class CPoisson1DItem;
};



#endif // !defined(AFX_POISSON1D_H__6933662D_63CF_41CE_977C_41A8B0661205__INCLUDED_)
