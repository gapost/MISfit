#ifndef _MISINT_H_
#define _MISINT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "qit.h"

#define TYPE_NCL 0
#define TYPE_PCL 1
#define TYPE_NQ 2
#define TYPE_PQ 3

class CMISInterp 
{
public:
	CMISInterp(int atype = 0, float aCox = 1e-6, float aLogNd = 16);
	virtual ~CMISInterp() {}
	
	virtual void SetType(int atype) { type = atype; }
	virtual void SetLogNd(float aLogNd) { LogNd = aLogNd; }
	void SetCox(float aCox) { Cox = aCox; }
	void SetVfb(float aVfb) { Vfb = aVfb; }
	
	float Ctot(float V, float& Vs);
	
	
protected:
	float LogNd,Cox,Vfb;
	int type;
	static  CSpliner2D<float> QNdCl_;
	static  CSpliner2D<float> CNdCl_;
	static  CSpliner2D<float> QNaCl_;
	static  CSpliner2D<float> CNaCl_;
	
	static  CSpliner2D<float> QNdQ_;
	static  CSpliner2D<float> CNdQ_;
	static  CSpliner2D<float> QNaQ_;
	static  CSpliner2D<float> CNaQ_;
	
	virtual float Qs_(float Vs);
    float Vs_(float V);
	float Cs_(float Vs);
};

class CMISitInterp : public CMISInterp 
{
public:
	CMISitInterp(CQit4* aQit, int atype = 0, float aCox = 1e-6, float aLogNd = 16) :
	  CMISInterp(atype,aCox,aLogNd), Qit(aQit)
	  { CP_(); }
	virtual ~CMISitInterp() 
	{ if (Qit) delete Qit; }
	
	virtual void SetType(int atype) { type = atype; CP_(); }
	virtual void SetLogNd(float aLogNd) { LogNd = aLogNd; CP_(); }

	cdouble Ctot(float V, float omega, float& Vs);

	CQit4& GetQit() { return *Qit; }
	
	
protected:
	CQit4* Qit;
	virtual float Qs_(float Vs);
	void CP_();
	float cp_;
};


#define PAR_VFB 0
#define PAR_COX 1
#define PAR_LND 2

class CMISFit : public CLMFit, CMISInterp
{
public:
	CMISFit(const CArray1D& aVg, const CArray1D& aC, int atype) : 
	  CLMFit(aVg,aC,3), CMISInterp(atype) {}
	virtual void MinFunc(CArray1D& x, CArray1D& y, int& flag)
	{
		CheckParams(x);
		SetVfb(x[PAR_VFB]);
		SetCox(x[PAR_COX]);
		SetLogNd(x[PAR_LND]);
		float b;
		for(int i=0; i<m; i++) y[i] = ydata[i] - 1e6*Ctot(xdata[i],b);
	}
	virtual void CheckParams(CArray1D& apar)
	{
		CLMFit::CheckParams(apar);
		if (apar[PAR_COX]<1e-9) apar[1] = 1e-9;
		if (apar[PAR_COX]>1e-5) apar[1] = 5e-6;
		if (apar[PAR_LND]<13.) apar[1] = 13.;
		if (apar[PAR_LND]>18.) apar[1] = 18.;
	}
	virtual void SetScales(CArray1D& ScaleFactors)
	{
		ScaleFactors[PAR_COX] = 1e6;
		ScaleFactors[PAR_VFB] = 1.;
		ScaleFactors[PAR_LND] = 0.01;
	}
protected:
	int mis_type;
};

#endif 
