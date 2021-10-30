// MIS.h: interface for the CMIS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIS_H__942E580E_4A19_4CCB_A196_D0B178B94BE9__INCLUDED_)
#define AFX_MIS_H__942E580E_4A19_4CCB_A196_D0B178B94BE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Poisson1D.h"

#define QUALITY_LOW 0
#define QUALITY_NORMAL 1
#define QUALITY_HIGH 2


class CMIS
{
public:
	CMIS()  : Vb_(0), isok(0) {}
	virtual ~CMIS() {}

	virtual double CalcChf() { return 0.; }
	virtual cdouble CalcC(double freq) { return 0.; }
	virtual double CalcClf() { return 0.; }
	virtual double CalcI() { return 0.; }
	virtual double SetVb(double aVb) { return 0.; }
	double Vb() { return Vb_; }
	double Cox() { return Cox_; }
	double Vs() { return (*(pBCs->get_v()) - *(pBCs->get_cp()))/(pBCs->get_beta()); }
	double Efs() { return -(*(pBCs->get_v()))/(pBCs->get_beta()); }
	double CPs() { return (*(pBCs->get_cp()))/(pBCs->get_beta()); }
	double CP0() { return (*(pBC0->get_cp()))/(pBC0->get_beta()); }
	double Fs() { return pBCs->GetRightF(); }
	double Fox() { return pBC0->GetRightF(); }
	double Qit() { return pBCs->GetVb(); }

	CPoissonBC* pBC0;
	CPoissonBC* pBCs;
	CPoissonBC* pBC1;
protected:
	double Vb_,Cox_;
	int isok;
};


class CClMIS : public CMIS  
{
public:
	CClMIS(double atox, double akox, double aNd, double aNa, CQitArray* aQit = 0, double aQf = 0.,
		bool fermi = 1, int quality = QUALITY_NORMAL, double T = 300.);
	virtual ~CClMIS() {}

	virtual double CalcChf() { return P.CalcChf(); }
	virtual double CalcClf() { return P.CalcClf(); }
	virtual cdouble CalcC(double freq) { return P.CalcC(freq); }
	virtual double SetVb(double aVb);
	CPoisson1D& GetPoisson1D() { return P; }
protected:
	CClMIS() {}
	CPoisson1D P;
	bool bFermi;
};

class CClGeMIS : public CMIS  
{
public:
	CClGeMIS(double atox, double akox, double aNd, double aNa, CQitArray* aQit = 0, double aQf = 0.,
		bool fermi = 1, int quality = QUALITY_NORMAL, double T = 300.);
	virtual ~CClGeMIS() {}

	virtual double CalcChf() { return P.CalcChf(); }
	virtual double CalcClf() { return P.CalcClf(); }
	virtual cdouble CalcC(double freq) { return P.CalcC(freq); }
	virtual double SetVb(double aVb);
	CPoisson1D& GetPoisson1D() { return P; }
protected:
	CClGeMIS() {}
	CPoisson1D P;
	bool bFermi;
};


class CQMIS : public CMIS 
{
public:
	CQMIS(double atox, double akox, double aNd, double aNa, CQitArray* aQit = 0, double aQf = 0.,
		bool fermi = 1, int quality = QUALITY_NORMAL, double T = 300.);
	virtual ~CQMIS() {}

	virtual double CalcChf() { return P.CalcChf(); }
	virtual double CalcClf() { return P.CalcClf(); }
	virtual cdouble CalcC(double freq) { return P.CalcC(freq); }
	virtual double SetVb(double aVb);
	CSCPS& GetSCPS() { return P; }
protected:
	CSCPS P;
};

class CQGeMIS : public CMIS 
{
public:
	CQGeMIS(double atox, double akox, double aNd, double aNa, CQitArray* aQit = 0, double aQf = 0.,
		bool fermi = 1, int quality = QUALITY_NORMAL, double T = 300.);
	virtual ~CQGeMIS() {}

	virtual double CalcChf() { return P.CalcChf(); }
	virtual double CalcClf() { return P.CalcClf(); }
	virtual cdouble CalcC(double freq) { return P.CalcC(freq); }
	virtual double SetVb(double aVb);
	CSCPS& GetSCPS() { return P; }
protected:
	CSCPS P;
};


class COpenMISSchrodinger1D : public COpenSchrodinger1D
{
public:
	void SetNox(int aNox) { Nox = aNox; }
	COpenMISSchrodinger1D(int asign=1, double T = 300.) : COpenSchrodinger1D(asign,T), Nox(0) {}
protected:
	virtual bool IsValidEigenfunction(int i);
	int Nox;

};

class CQTMIS : public CMIS 
{
public:
	CQTMIS(double atox, double akox, double mox, 
		double dEc, double dEv, 
		double aNd, double aNa, CQitArray* aQit = 0, double Qf = 0.,
		bool fermi = 1, int quality = QUALITY_NORMAL, double T = 300.);
	virtual ~CQTMIS() {}

	virtual double CalcChf() { return P.CalcChf(); }
	virtual double CalcClf() { return P.CalcClf(); }
	virtual double SetVb(double aVb);
	CSCPS& GetSCPS() { return P; }
	virtual double CalcI() { return P.CalcI(); }
protected:
	CSCPS P;
};


#endif // !defined(AFX_MIS_H__942E580E_4A19_4CCB_A196_D0B178B94BE9__INCLUDED_)
