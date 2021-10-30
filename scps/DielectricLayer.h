#ifndef DIELECTRICLAYER_H
#define DIELECTRICLAYER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Poisson1DItem.h"

class CDielectricLayer : public CPoisson1DLayer
{
public:
	CDielectricLayer(CGrid* agrid, double akappa, double aQf = 0.) 
		: CPoisson1DLayer(agrid,akappa),
		Qf(aQf/N0)
	{}
	virtual ~CDielectricLayer() {}

	virtual void CalcQ(int hf=0);
	virtual void SetBeta(double abeta) { Qf /= beta; beta = abeta; Qf *= beta; }
protected:
	double Qf;
};

class CQuantumDielectricLayer : public CDielectricLayer
{
public:
	CQuantumDielectricLayer(CGrid* agrid, double akappa, double aQf = 0.) 
		: CDielectricLayer(agrid,akappa,aQf)
	{ isquant = true; }
	virtual ~CQuantumDielectricLayer() {}

	virtual void CalcQ(int hf=0);
};

class CSemiconductorLayer : public CPoisson1DLayer
{
public:
	CSemiconductorLayer(CGrid* agrid, double akappa,
		double aEc, double aEv, double aEd, double aEa,
		double aNc, double aNv, double aNd, double aNa, 
		bool afermi=1) 
		: CPoisson1DLayer(agrid,akappa), 
		Ec(aEc), Ev(aEv), Ed(aEd), Ea(aEa),
		Nc(aNc/N0), Nv(aNv/N0), Nd(aNd/N0), Na(aNa/N0),
		fermi(afermi)
	{}
	virtual ~CSemiconductorLayer() {}

	//virtual void PrepareForCalcC(double dv);

	virtual void CalcQ(int hf=0);
	virtual void CalcCP();
	virtual int FreezeWhat() { if (Nd>Na) return HOLES; else return ELECTRONS; }

	virtual void SetBeta(double abeta);

protected:
	virtual void CalcQ_(int i, ChargeArray& pq, ChargeArray& pdq, int ifreeze=0);

	bool fermi; // how to calculate Fermi integrals
	double Ec,Ev, // bands in eV
		   Ed,Ea,
		   Nc,Nv,
		   Nd,Na; // doping in cm^-3
};

class CQuantumSemiconductorLayer : public CSemiconductorLayer
{
public:
	CQuantumSemiconductorLayer(CGrid* agrid, double akappa,
		double aEc, double aEv, double aEd, double aEa,
		double aNc, double aNv, double aNd, double aNa) 
		: CSemiconductorLayer(agrid,akappa,aEc,aEv,aEd,aEa,aNc,aNv,aNd,aNa) 
	{ isquant = true; }

protected:
	virtual void CalcQ_(int i, ChargeArray& pq, ChargeArray& pdq, int ifreeze=0);
};



#endif // !defined(AFX_POISSON1D_H__6933662D_63CF_41CE_977C_41A8B0661205__INCLUDED_)
