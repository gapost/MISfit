// S1D.h: interface for the CS1D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_S1D_H__EBA689F7_9696_424D_A0CA_39FCC58717A3__INCLUDED_)
#define AFX_S1D_H__EBA689F7_9696_424D_A0CA_39FCC58717A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "constants.h"
#include "Schrodinger1DItem.h"

#include <vector>

class CSchrodinger1D  
{
public:
	CSchrodinger1D(int asign = 1, double T = 300.) : beta(300/T/KT300), sign(asign), ne(0), 
		First(0), Last(0), nItem(0) {}
	virtual ~CSchrodinger1D();

	void Add(CSchrodinger1DItem* P);
	int get_sign() { return sign; }
	int get_beta() { return beta; }

	virtual void Init(double xo = 0.);
	virtual void Init(int aM, double* ax, double* ah, double* aL, double* av, double* aq, double* adq, double* adelta);
	virtual int Solve(double eps = 1e-6);
	virtual int Solve(double lbound, double ubound);
	virtual void ProcessEigenfuncs();

	virtual double get_eigenval(int i) { return E[i]/beta; }
	virtual double get_eigengamma(int i) { return 0.; }

	virtual double CalcI() { return 0; }
	virtual void AddI(double& I) { }
	void CalcQ();
	void AddQ();
	void CalcDelta();
	void AddDelta();
	void CalcNs(double& Ns, double& xd);
	void Dump(const char* s);

	CArray1D x,h,L,v,q,dq; 
	CArray1D E;
	CArray2D Z;
	CArray1D Md, absMd;
	CArray2D Delta;
	int ne;
	int M;
protected:
	CArray1D d1; 
	double beta;
	int sign;
	CTridiagSym2D T;
	CArray1D W1,W2;
	CArray2D G;

	CSchrodinger1DItem *First,*Last;
	int nItem;

	friend class CSchrodinger1DItem;
	friend class CMultiBandSchrodinger1D;  
};

class COpenSchrodinger1D : public CSchrodinger1D
{
public:
	COpenSchrodinger1D(int asign = 1, double T = 300.) : CSchrodinger1D(asign,T) {}
	virtual ~COpenSchrodinger1D() {}

	virtual void Init(double xo = 0.);
	virtual void Init(int aM, double* ax, double* ah, double* aL, double* av, double* aq, double* adq, double* adelta);
	virtual void ProcessEigenfuncs();

	virtual double get_eigengamma(int i) { return Gamma[i]/beta; }

	virtual double CalcI();
	virtual void AddI(double& I);

	CArray1D Gamma;

protected:
	CComplexTridiag2D H;
	CComplexArray1D b,b1;

	virtual bool IsValidEigenfunction(int i) { return true; }
	void RefineEigenFunctions();

	friend class CSchrodinger1DItem;
	friend class CMultiBandSchrodinger1D;  
};

typedef std::vector<CSchrodinger1D*> CSchrodinger1DArray;

class CMultiBandSchrodinger1D
{
public:
	void FixTail2();
	CMultiBandSchrodinger1D(int asign = 1, double T=300.) : 
	  sign(asign),
	  beta(300/T/KT300), 
	  nbands(0),
	  Ns(0.), xs(0.)
	  {
	  }
	virtual ~CMultiBandSchrodinger1D();

	int get_nbands() { return nbands; }

	void AddBand(CSchrodinger1D* P) 
	{ 
		assert(P->get_sign()==sign);
		//assert(P->get_beta()==beta);
		Bands.push_back(P); 
		nbands++;
	}
	void Add(int iband, CSchrodinger1DItem* P) 
	{ 
		assert(iband<nbands); 
		assert(P->get_sign()==Bands.at(iband)->get_sign());
		Bands.at(iband)->Add(P); 
	}
	CSchrodinger1D* GetBand(int iband) { return Bands[iband]; }

	void Init(double xo = 0.);
	void Init(int aM, double* ax, double* ah, double* aL, double* av, double* aq, double* adq, double* adelta);
	int Solve(double eps = 1e-6);

	void FixTail(double xm, double qcl, double dqcl,
				double xm1, double qcl1, double qcl2);

	double CalcI();
	void CalcQ();
	void CalcDelta();
	void CalcNs();
	void Dump(const char* s);

	CArray1D x,h,L,v,q,dq;
	CArray2D Delta;
	double Ns,xs;
protected:
	int nbands;
	double beta;
	int ne;
	int sign;
	bool fixed_tails;

	CSchrodinger1DArray Bands;
};

#endif // !defined(AFX_S1D_H__EBA689F7_9696_424D_A0CA_39FCC58717A3__INCLUDED_)
