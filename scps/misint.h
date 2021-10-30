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

class CInterpSemiconductorData
{
public:
	virtual float Cs(float Vs, float LogNd, int type) { return 0; }
	virtual float Qs(float Vs, float LogNd, int type) { return 0; }
	virtual float Cp(float Vs, float LogNd, int type) { return 0; }
};

class CMISInterpBase 
{
public:
	CMISInterpBase(
		CInterpSemiconductorData* aData,
		int atype = 0, float aCox = 1e-6, float aLogNd = 16, float Vfb = 0.f);
	virtual ~CMISInterpBase() {}
	
	virtual void SetType(int atype) { type = atype; }
	virtual void SetLogNd(float aLogNd) { LogNd = aLogNd; }

	void SetCox(float aCox) { cox_ = aCox; }
	void SetVfb(float aVfb) { Vfb = aVfb; }
	
	float Ctot(float Vg);
	float Cox() { return cox_; }
	
	float Vs() { return vs_; }

	virtual float Qtot(float Vs) { return pSemiconductorData->Qs(Vs,LogNd,type); }

	float Qs(float Vs) { return pSemiconductorData->Qs(Vs,LogNd,type); }
	float Cs(float Vs) { return pSemiconductorData->Cs(Vs,LogNd,type); }
	
protected:
	CInterpSemiconductorData* pSemiconductorData;
	float LogNd,cox_,Vfb;
	int type;
    float Vs_(float Vg);
	virtual float Vox(float Vg, float Vs) { return Vg - Vs; }
	float vs_;
};

class CMISitInterpBase : public CMISInterpBase 
{
public:
	CMISitInterpBase(CQitArray* aQit, CInterpSemiconductorData* aData,
		int atype = 0, float aCox = 1e-6, float aLogNd = 16, float aVfb = 0.f) :
	  CMISInterpBase(aData,atype,aCox,aLogNd,aVfb), pQit(aQit)
	  { CP_(); VS0(); vts_ = Qit(0)/cox_ - vs0_; }
	virtual ~CMISitInterpBase() { }
	
	virtual void SetType(int atype) { type = atype; CP_(); VS0(); }
	virtual void SetLogNd(float aLogNd) { LogNd = aLogNd; CP_(); VS0(); }

	cdouble Ctot(float V, float omega);
	cdouble Cit(float Vs, float omega) {
		return pQit->Cit((double)(Vs+cp_)/KT300,(double)omega);
	}

	float Efs() { return - cp_ - vs_; }

	CQitArray* GetQit() { return pQit; }

	float GetCP() { return cp_; }
	float GetTrapShift() { return vts_; }
	
	float Qit(float Vs);
	virtual float Qtot(float Vs) { return pSemiconductorData->Qs(Vs,LogNd,type)+Qit(Vs); }
	
protected:
	CQitArray* pQit;
	void CP_();
	void VS0();
	float cp_,vs0_,vts_;
	virtual float Vox(float Vg, float Vs) { return Vg - Vs + vs0_; }
};
// Si
class CMISInterp : public CMISInterpBase 
{
public:
	CMISInterp(int atype = 0, float aCox = 1e-6, float aLogNd = 16, float Vfb = 0.f);
};
class CMISitInterp : public CMISitInterpBase 
{
public:
	CMISitInterp(CQitArray* aQit, int atype = 0, float aCox = 1e-6, float aLogNd = 16, float Vfb = 0.f);
};
// Ge
class CGeMISInterp : public CMISInterpBase 
{
public:
	CGeMISInterp(int atype = 0, float aCox = 1e-6, float aLogNd = 16, float Vfb = 0.f);
};
class CGeMISitInterp : public CMISitInterpBase 
{
public:
	CGeMISitInterp(CQitArray* aQit, int atype = 0, float aCox = 1e-6, float aLogNd = 16, float Vfb = 0.f);
};
// GaAs
class CGaAsMISInterp : public CMISInterpBase 
{
public:
	CGaAsMISInterp(int atype = 0, float aCox = 1e-6, float aLogNd = 16, float Vfb = 0.f);
};
class CGaAsMISitInterp : public CMISitInterpBase 
{
public:
	CGaAsMISitInterp(CQitArray* aQit, int atype = 0, float aCox = 1e-6, float aLogNd = 16, float Vfb = 0.f);
};
/*
class CMISitInterp : public CMISitInterpBase 
{
public:
	CMISitInterp(CQitArray* aQit, int atype = 0, float aCox = 1e-6, float aLogNd = 16, float Vfb = 0.f) :
	  CMISitInterpBase(aQit,atype,aCox,aLogNd,Vfb)
	  {}
	virtual ~CMISitInterp() {}
	
	virtual float Cs(float Vs);
	virtual float Qs(float Vs);
	
protected:
	static  CSpliner2D<float> QNdCl_;
	static  CSpliner2D<float> CNdCl_;
	static  CSpliner2D<float> QNaCl_;
	static  CSpliner2D<float> CNaCl_;
	
	static  CSpliner2D<float> QNdQ_;
	static  CSpliner2D<float> CNdQ_;
	static  CSpliner2D<float> QNaQ_;
	static  CSpliner2D<float> CNaQ_;
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
		for(int i=0; i<m; i++) y[i] = ydata[i] - Ctot(xdata[i]);
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
*/

#endif 
