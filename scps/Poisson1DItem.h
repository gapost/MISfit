#ifndef POISSON1DITEM_H
#define POISSON1DITEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "constants.h"
#include "qit.h"
#include "grid.h"

#define N_CHARGE_TYPES 4
#define Q_SUM 0
#define FIXED_CHARGE 1
#define ELECTRONS 2
#define HOLES 3

typedef double ChargeArray[N_CHARGE_TYPES];

class CPoisson1D;
class CSCPS;

class CPoisson1DItem
{
public:
	CPoisson1DItem() : 
	x(0), h(0), v(0), cp(0), 
	q(0), dq(0), 
	Td(0), Ts(0), Ad(0), As(0),
	Cd(0), Ce(0), Cs(0), b(0),
	Prev(0), Next(0),
	beta(1),
	isquant(false),
	qh(0), dqh(0), qe(0), dqe(0),
	nse(0), xse(0),
	nsh(0), xsh(0)
	{}
	virtual ~CPoisson1DItem() {}

	virtual int get_m() { return 1; }
	bool is_quant() { return isquant; }
	double* get_v() { return v; }
	double* get_cp() { return cp; }

	virtual void MakeGrid() {  }
	virtual void MakeMatrix() { }
	virtual void PrepareForSolve() {  }
	virtual void PrepareForCalcC(int hf=0) { }
	virtual void PrepareForCalcC(double omega) { }
	virtual void PrepareForCalcC(int hf, CArray2D& De, CArray2D& Dh, CArray2D& D) { }
	virtual void PrepareForCalcC(double omega, CArray2D& De, CArray2D& Dh, CArray2D& D) { }
	virtual int FreezeWhat() { return -1; }
	virtual void FixBoundaryCharge() {}

	virtual void CalcQ(int hf=0)  { }
	virtual void CalcCP() { } 

	virtual void Dump(ostream &s) { } 

	virtual double GetRightD() { return 0.; }
	virtual double GetLeftD() { return 0.; }
	virtual double GetRightF() { return 0.; }
	virtual double GetLeftF() { return 0.; }

	virtual void SetBeta(double abeta) { beta = abeta; }
	double get_beta() { return beta; }

	void Assign(CPoisson1D* P, int& i);
	void Assign(CSCPS* P, int& ip, int& is);

	CPoisson1DItem* GetNext() { return Next; }
	CPoisson1DItem* GetPrev() { return Prev; }
	void SetNext(CPoisson1DItem* P) { Next = P; }
	void SetPrev(CPoisson1DItem* P) { Prev = P; }

protected:
	double beta;

	CPoisson1DItem *Prev,*Next;

	double *x, *h, *v, *cp;
	ChargeArray *q, *dq;
	double *Td,*Ts,*Ad,*As;
	double *Cd,*Ce,*Cs,*b;
	cdouble *cCd,*cCe,*cCs,*cb;

	CPoisson1D* pPoisson;

	bool isquant;
	int ip, is;
	double *qh, *dqh, *qe, *dqe;
	double *nse,*nsh,*xse,*xsh;

	friend class CPoissonBC;
};

class CPoissonBC : public CPoisson1DItem
{
public:
	typedef enum { field, voltage, cont } PoissonBoundaryType;

	CPoissonBC(PoissonBoundaryType abtype, double aVb = 0.) : btype(abtype), dVb(0.)
	{
		SetVb(aVb);
		for(int i=0; i<N_CHARGE_TYPES; i++) {
			q0[i] = 0.;
			dq0[i] = 0.;
			q1[i] = 0.;
			dq1[i] = 0.;
		}
	}
	virtual ~CPoissonBC() {}

	virtual void MakeMatrix();
	virtual void PrepareForSolve();
	virtual void PrepareForCalcC(int hf=0);
	virtual void PrepareForCalcC(double omega);
	virtual void PrepareForCalcC(int hf, CArray2D& De, CArray2D& Dh, CArray2D& D);
	virtual void PrepareForCalcC(double omega, CArray2D& De, CArray2D& Dh, CArray2D& D);
	virtual void FixBoundaryCharge();

	virtual void Dump(ostream &s);

	virtual double GetRightD();
	virtual double GetLeftD();
	virtual double GetRightF();
	virtual double GetLeftF();

	virtual void SetBeta(double abeta) { Vb /= beta; beta = abeta; Vb *= beta; }

	void SetVb(double aVb) { Vb=beta*aVb; if (btype==cont) Vb /= (N0*L0); }
	double GetVb() { double r = Vb/beta; if (btype==cont) r *= (N0*L0); return r; }

	ChargeArray& LeftQ() {	if (btype==cont/* && !isquant*/) return q0; else return *q; }
	ChargeArray& LeftdQ() { if (btype==cont/* && !isquant*/) return dq0; else return *dq; }
	ChargeArray& RightQ() { if (btype==cont/* && !isquant*/) return q1; else return *q; }
	ChargeArray& RightdQ() { if (btype==cont/* && !isquant*/) return dq1; else return *dq; }

	PoissonBoundaryType btype;

protected:
	double Vb,dVb;
	ChargeArray q0,dq0,q1,dq1;
};

class CPoissonBC_Dit : public CPoissonBC
{
public:
	CPoissonBC_Dit(CQitArray* aQit) : 
	CPoissonBC(CPoissonBC::cont), pQit(aQit)
	{	}
	virtual void SetBeta(double abeta) { 
		pQit->set_beta(abeta); 
		beta = abeta; 
	}
	virtual void FixBoundaryCharge();
	virtual void PrepareForCalcC(double omega);
	virtual void PrepareForCalcC(double omega, CArray2D& De, CArray2D& Dh, CArray2D& D);
protected:
	CQitArray* pQit;
};

class CQuantumPoissonBC : public CPoissonBC
{
public:
	CQuantumPoissonBC(PoissonBoundaryType abtype, double aVb = 0.) : 
	CPoissonBC(abtype,aVb)
	{
		isquant = true;
	}
	bool is_quant_cl_boundary()
	{
		if (Prev==0 || Next==0) return false;
		return (Prev->is_quant() && !Next->is_quant()) ||
			(!Prev->is_quant() && Next->is_quant());
	}
};

class CQuantumPoissonBC_Dit : public CPoissonBC_Dit
{
public:
	CQuantumPoissonBC_Dit(CQitArray* aQit) : 
	CPoissonBC_Dit(aQit)
	{
		isquant = true;
	}
	bool is_quant_cl_boundary()
	{
		if (Prev==0 || Next==0) return false;
		return (Prev->is_quant() && !Next->is_quant()) ||
			(!Prev->is_quant() && Next->is_quant());
	}
};

class CPoisson1DLayer : public CPoisson1DItem
{
public:
	CPoisson1DLayer(CGrid* agrid, double akappa) 
		: grid(agrid), kappa(akappa),  F(agrid->get_m() + 1)  
	{}
	virtual ~CPoisson1DLayer() { delete grid; }

	virtual int get_m() { return grid->get_m(); }
	virtual void MakeGrid() { grid->MakeGrid(x); }
	virtual void MakeMatrix();
	virtual void PrepareForSolve();
	virtual void PrepareForCalcC(int hf=0);
	virtual void PrepareForCalcC(double omega);
	virtual void PrepareForCalcC(int hf, CArray2D& De, CArray2D& Dh, CArray2D& D);
	virtual void PrepareForCalcC(double omega, CArray2D& De, CArray2D& Dh, CArray2D& D);
	
	virtual void CalcQ(int hf=0);
	virtual void CalcCP();

	virtual void Dump(ostream &s);

	void CalcF();

	double kappa;
protected:
	CArray1D F;
	CGrid* grid;
};


#endif // !defined(AFX_POISSON1D_H__6933662D_63CF_41CE_977C_41A8B0661205__INCLUDED_)
