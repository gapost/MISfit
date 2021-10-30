// Schrodinger1DItem.h: interface for the CSchrodinger1DItem class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SCHROD1DITEM_H
#define SCHROD1DITEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "constants.h"
#include "grid.h"

class CSchrodinger1D;

class CSchrodinger1DItem  
{
public:
	CSchrodinger1DItem(int asign = 1) : 
	  sign(asign), Prev(0), Next(0), beta(1) 
	  { assert(asign==1 || asign==-1); }
	virtual ~CSchrodinger1DItem() {}

	void Assign(CSchrodinger1D* pS,	int& i);

	virtual int get_m() { return 1; }
	int get_sign() { return sign; }

	virtual void MakeMatrix() {}
	virtual void CalcQ() {}
	virtual void CalcDelta() {}
	virtual void MakeGrid() {}
	virtual void SetBeta(double abeta) { beta = abeta; }
	virtual void CalcBounds(double& lbound, double& ubound, double eps = 1e-6) { }
	virtual void Dump(ostream& s) {}
	virtual double get_Md() { return 0.; } 
	
	CSchrodinger1DItem* GetNext() { return Next; }
	CSchrodinger1DItem* GetPrev() { return Prev; }
	void SetNext(CSchrodinger1DItem* P) { Next = P; }
	void SetPrev(CSchrodinger1DItem* P) { Prev = P; }

protected:
	double *x,*L,*v,*q,*dq,*h; 
	double *d,*d1,*s;
	CArray1D E,W1,W2;
	CArray2D Z;
	int* ne;
	int iequ;
	CSchrodinger1DItem *Prev,*Next;
	double beta;
	int sign;
};

class CSchrodingerBC : public CSchrodinger1DItem
{
public:
	typedef enum { box, transmit, cont } SchrodingerBoundaryType;

	CSchrodingerBC(SchrodingerBoundaryType atype, int asign = 1): 
	  CSchrodinger1DItem(asign), btype(atype) {}
	virtual ~CSchrodingerBC() {}

  	virtual void MakeMatrix();
	virtual void CalcQ();
	virtual double get_Md();

	void CalcI(double& I);

	SchrodingerBoundaryType get_type() { return btype; }

protected:
	SchrodingerBoundaryType btype;
};

class CSchrodinger1DLayer : public CSchrodinger1DItem
{
public:
	CSchrodinger1DLayer(CGrid* agrid, double aMz = 1, double aMd = 1, 
		int g = 1, int asign = 1, double DE = 0.): 
	  CSchrodinger1DItem(asign),
		  grid(agrid),
		  Mz( asign*aMz/SCHROD_E0 ), 
		  Md( asign*g*aMd/(2*PI*SCHROD_E0)*(1e21/N0) ),
		  dE( DE )
	  { } 

	  virtual ~CSchrodinger1DLayer() { delete grid; }

	virtual int get_m() { return grid->get_m(); }
  	virtual void MakeMatrix();
	virtual void CalcQ();
	virtual void MakeGrid() { grid->MakeGrid(x); }
	virtual void SetBeta(double abeta) 
	{
		double c = abeta/beta;
		Mz/= c; dE *= c;
		beta = abeta;
	}
	virtual void CalcBounds(double& lbound, double& ubound, double eps = 1e-6);
	virtual void Dump(ostream& s);
	virtual double get_Md() { return Md; } 

	double Mz,Md,dE;
	CGrid* grid;
};

#endif 
