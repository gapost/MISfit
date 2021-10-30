#include "stdafx.h"

#include "FermiFunc.h"

#define VMAX 2.5
#define NMIN 13
#define NMAX 18
#define NV 151
#define NN 21

#include "misint.h"

/********* MIS Base Implementation **********/

CMISInterpBase::CMISInterpBase(CInterpSemiconductorData* aData, 
							   int atype, float aCox, float aLogNd, float aVfb) :
cox_(aCox), LogNd(aLogNd), Vfb(aVfb), type(atype),
pSemiconductorData(aData)
{}

float CMISInterpBase::Vs_(float Vg)
{
	float x1 , x2;
	{ x2 = VMAX; x1 = -VMAX; }
	
	
	int j,JMAX=40;
	float dx,f,fmid,xmid,rtb;
	
	
	f = Vox(Vg,x1)*cox_ - Qtot(x1);
	fmid = Vox(Vg,x2)*cox_ - Qtot(x2);
	if (f*fmid >= 0.0) {
		//assert(1==0);
		return VMAX * Vg / fabs(Vg);
	}
	rtb = f < 0.0 ? (dx=x2-x1,x1) : (dx=x1-x2,x2); // Orient the search so that f>0 lies at x+dx.
	for (j=1;j<=JMAX;j++) {
		xmid=rtb+(dx *= 0.5);
		fmid=Vox(Vg,xmid)*cox_ - Qtot(xmid); //Bisection loop.
		if (fmid <= 0.0) rtb=xmid;
		if (fabs(dx) < 1e-3 || fmid == 0.0) return rtb;
	}
	//assert(1==0);
	return VMAX * Vg / fabs(Vg); //Never get here.
}

float CMISInterpBase::Ctot(float Vg)
{
	vs_ = Vs_(- (Vg-Vfb));
	float cs = pSemiconductorData->Cs(vs_,LogNd,type);
	return cs*cox_/(cs+cox_);
}

/********* MIS Base Implementation with Qit **********/

float CMISitInterpBase::Qit(float Vs)
{
	double Qi,dQi;
	pQit->Qit((Vs+cp_)/KT300,Qi,dQi);
	return Qi;
}

cdouble CMISitInterpBase::Ctot(float Vg, float omega)
{
	vs_ = Vs_(- (Vg-Vfb) + vts_); // Vs_(-(Vg-Vfb));
	cdouble cit_ = Cit(vs_,omega);
	cit_ += pSemiconductorData->Cs(vs_,LogNd,type);
	return cit_*(double)cox_/(cit_+(double)cox_);
}

void CMISitInterpBase::CP_()
{
	cp_ = pSemiconductorData->Cp(0,LogNd,type);
}

void CMISitInterpBase::VS0()
{
	float x1 , x2;
	{ x2 = VMAX; x1 = -VMAX; }
	
	
	int j,JMAX=40;
	float dx,f,fmid,xmid,rtb;
	
	
	f = Qtot(x1);
	fmid = Qtot(x2);
	if (f*fmid >= 0.0) {
		//assert(1==0);
		vs0_=0.f;
		return;
	}
	rtb = f < 0.0 ? (dx=x2-x1,x1) : (dx=x1-x2,x2); // Orient the search so that f>0 lies at x+dx.
	for (j=1;j<=JMAX;j++) {
		xmid=rtb+(dx *= 0.5);
		fmid = Qtot(xmid); //Bisection loop.
		if (fmid <= 0.0) rtb=xmid;
		if (fabs(dx) < 1e-3 || fmid == 0.0) { vs0_=rtb; return; }
	}
	//assert(1==0);
	vs0_=0.f;
	return; //Never get here.
}

/********** Si MIS ***********/

class CInterpSi : public CInterpSemiconductorData
{
public:
	virtual float Cs(float Vs, float LogNd, int type);
	virtual float Qs(float Vs, float LogNd, int type);
	virtual float Cp(float Vs, float LogNd, int type);
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

#include "..\gentbl\inc\V.h"
#include "..\gentbl\inc\logNd.h"

#include "..\gentbl\inc\QNdCl.h"
#include "..\gentbl\inc\CNdCl.h"

#include "..\gentbl\inc\QNaCl.h"
#include "..\gentbl\inc\CNaCl.h"

#include "..\gentbl\inc\QNdQ.h"
#include "..\gentbl\inc\CNdQ.h"

#include "..\gentbl\inc\QNaQ.h"
#include "..\gentbl\inc\CNaQ.h"

CSpliner2D<float> CInterpSi::QNdCl_(NN,NV,logNd,V,&Q_Nd_Cl[0][0]);
CSpliner2D<float> CInterpSi::CNdCl_(NN,NV,logNd,V,&C_Nd_Cl[0][0]);
CSpliner2D<float> CInterpSi::QNaCl_(NN,NV,logNd,V,&Q_Na_Cl[0][0]);
CSpliner2D<float> CInterpSi::CNaCl_(NN,NV,logNd,V,&C_Na_Cl[0][0]);

CSpliner2D<float> CInterpSi::QNdQ_(NN,NV,logNd,V,&Q_Nd_Q[0][0]);
CSpliner2D<float> CInterpSi::CNdQ_(NN,NV,logNd,V,&C_Nd_Q[0][0]);
CSpliner2D<float> CInterpSi::QNaQ_(NN,NV,logNd,V,&Q_Na_Q[0][0]);
CSpliner2D<float> CInterpSi::CNaQ_(NN,NV,logNd,V,&C_Na_Q[0][0]);


float CInterpSi::Cs(float Vs, float LogNd, int type)
{
	switch (type)
	{
	case TYPE_NCL:
		return CNdCl_(LogNd,Vs);
		break;
	case TYPE_PCL:
		return CNaCl_(LogNd,Vs);
		break;
	case TYPE_NQ:
		return CNdQ_(LogNd,Vs);
		break;
	case TYPE_PQ:
		return CNaQ_(LogNd,Vs);
		break;
	}
	return 0.f;
}

float CInterpSi::Qs(float Vs, float LogNd, int type)
{
	switch (type)
	{
	case TYPE_NCL:
		return QNdCl_(LogNd,Vs);
		break;
	case TYPE_PCL:
		return QNaCl_(LogNd,Vs);
		break;
	case TYPE_NQ:
		return QNdQ_(LogNd,Vs);
		break;
	case TYPE_PQ:
		return QNaQ_(LogNd,Vs);
		break;
	}
	return 0.f;
}

float CInterpSi::Cp(float Vs, float LogNd, int type)
{
	float Nd,Na;
	if (type==TYPE_NCL || type==TYPE_NQ) { Nd = pow(10.,LogNd); Na = 0.; }
	else { Nd = 0.; Na = pow(10.,LogNd); }

	return FindSemiconductorCP(0.56f,-0.56f,2.8e19f,1.04e19f,Nd,Na,(float)(1/KT300));
}

static CInterpSi InterpSi;

CMISInterp::CMISInterp(int atype, float aCox, float aLogNd, float Vfb) :
CMISInterpBase(&InterpSi,atype,aCox,aLogNd,Vfb)
{}

CMISitInterp::CMISitInterp(CQitArray* aQit, int atype, float aCox, float aLogNd, float Vfb) :
CMISitInterpBase(aQit, &InterpSi,atype,aCox,aLogNd,Vfb)
{}

/********** Ge MIS ***********/

class CInterpGe : public CInterpSemiconductorData
{
public:
	virtual float Cs(float Vs, float LogNd, int type);
	virtual float Qs(float Vs, float LogNd, int type);
	virtual float Cp(float Vs, float LogNd, int type);
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

#include "..\gentbl\inc\V_Ge.h"
#include "..\gentbl\inc\logNd_Ge.h"

#include "..\gentbl\inc\QNdCl_Ge.h"
#include "..\gentbl\inc\CNdCl_Ge.h"

#include "..\gentbl\inc\QNaCl_Ge.h"
#include "..\gentbl\inc\CNaCl_Ge.h"

#include "..\gentbl\inc\QNdQ_Ge.h"
#include "..\gentbl\inc\CNdQ_Ge.h"

#include "..\gentbl\inc\QNaQ_Ge.h"
#include "..\gentbl\inc\CNaQ_Ge.h"

CSpliner2D<float> CInterpGe::QNdCl_(NN,NV,logNd,V,&Q_Nd_Cl_Ge[0][0]);
CSpliner2D<float> CInterpGe::CNdCl_(NN,NV,logNd,V,&C_Nd_Cl_Ge[0][0]);
CSpliner2D<float> CInterpGe::QNaCl_(NN,NV,logNd,V,&Q_Na_Cl_Ge[0][0]);
CSpliner2D<float> CInterpGe::CNaCl_(NN,NV,logNd,V,&C_Na_Cl_Ge[0][0]);

CSpliner2D<float> CInterpGe::QNdQ_(NN,NV,logNd,V,&Q_Nd_Q_Ge[0][0]);
CSpliner2D<float> CInterpGe::CNdQ_(NN,NV,logNd,V,&C_Nd_Q_Ge[0][0]);
CSpliner2D<float> CInterpGe::QNaQ_(NN,NV,logNd,V,&Q_Na_Q_Ge[0][0]);
CSpliner2D<float> CInterpGe::CNaQ_(NN,NV,logNd,V,&C_Na_Q_Ge[0][0]);


float CInterpGe::Cs(float Vs, float LogNd, int type)
{
	switch (type)
	{
	case TYPE_NCL:
		return CNdCl_(LogNd,Vs);
		break;
	case TYPE_PCL:
		return CNaCl_(LogNd,Vs);
		break;
	case TYPE_NQ:
		return CNdQ_(LogNd,Vs);
		break;
	case TYPE_PQ:
		return CNaQ_(LogNd,Vs);
		break;
	}
	return 0.f;
}

float CInterpGe::Qs(float Vs, float LogNd, int type)
{
	switch (type)
	{
	case TYPE_NCL:
		return QNdCl_(LogNd,Vs);
		break;
	case TYPE_PCL:
		return QNaCl_(LogNd,Vs);
		break;
	case TYPE_NQ:
		return QNdQ_(LogNd,Vs);
		break;
	case TYPE_PQ:
		return QNaQ_(LogNd,Vs);
		break;
	}
	return 0.f;
}

float CInterpGe::Cp(float Vs, float LogNd, int type)
{
	float Nd,Na;
	if (type==TYPE_NCL || type==TYPE_NQ) { Nd = pow(10.,LogNd); Na = 0.; }
	else { Nd = 0.; Na = pow(10.,LogNd); }
	return FindSemiconductorCP(0.33f,-0.33f,1e19f,5e18f,Nd,Na,(float)(1/KT300));
}

static CInterpGe InterpGe;

CGeMISInterp::CGeMISInterp(int atype, float aCox, float aLogNd, float Vfb) :
CMISInterpBase(&InterpGe,atype,aCox,aLogNd,Vfb)
{}

CGeMISitInterp::CGeMISitInterp(CQitArray* aQit, int atype, float aCox, float aLogNd, float Vfb) :
CMISitInterpBase(aQit, &InterpGe,atype,aCox,aLogNd,Vfb)
{}

/********** GaAs MIS ***********/

class CInterpGaAs : public CInterpSemiconductorData
{
public:
	virtual float Cs(float Vs, float LogNd, int type);
	virtual float Qs(float Vs, float LogNd, int type);
	virtual float Cp(float Vs, float LogNd, int type);
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

#include "..\gentbl\inc\V_GaAs.h"
#include "..\gentbl\inc\logNd_GaAs.h"

#include "..\gentbl\inc\QNdCl_GaAs.h"
#include "..\gentbl\inc\CNdCl_GaAs.h"

#include "..\gentbl\inc\QNaCl_GaAs.h"
#include "..\gentbl\inc\CNaCl_GaAs.h"

#include "..\gentbl\inc\QNdQ_GaAs.h"
#include "..\gentbl\inc\CNdQ_GaAs.h"

#include "..\gentbl\inc\QNaQ_GaAs.h"
#include "..\gentbl\inc\CNaQ_GaAs.h"

CSpliner2D<float> CInterpGaAs::QNdCl_(NN,NV,logNd,V,&Q_Nd_Cl_GaAs[0][0]);
CSpliner2D<float> CInterpGaAs::CNdCl_(NN,NV,logNd,V,&C_Nd_Cl_GaAs[0][0]);
CSpliner2D<float> CInterpGaAs::QNaCl_(NN,NV,logNd,V,&Q_Na_Cl_GaAs[0][0]);
CSpliner2D<float> CInterpGaAs::CNaCl_(NN,NV,logNd,V,&C_Na_Cl_GaAs[0][0]);

CSpliner2D<float> CInterpGaAs::QNdQ_(NN,NV,logNd,V,&Q_Nd_Q_GaAs[0][0]);
CSpliner2D<float> CInterpGaAs::CNdQ_(NN,NV,logNd,V,&C_Nd_Q_GaAs[0][0]);
CSpliner2D<float> CInterpGaAs::QNaQ_(NN,NV,logNd,V,&Q_Na_Q_GaAs[0][0]);
CSpliner2D<float> CInterpGaAs::CNaQ_(NN,NV,logNd,V,&C_Na_Q_GaAs[0][0]);


float CInterpGaAs::Cs(float Vs, float LogNd, int type)
{
	switch (type)
	{
	case TYPE_NCL:
		return CNdCl_(LogNd,Vs);
		break;
	case TYPE_PCL:
		return CNaCl_(LogNd,Vs);
		break;
	case TYPE_NQ:
		return CNdQ_(LogNd,Vs);
		break;
	case TYPE_PQ:
		return CNaQ_(LogNd,Vs);
		break;
	}
	return 0.f;
}

float CInterpGaAs::Qs(float Vs, float LogNd, int type)
{
	switch (type)
	{
	case TYPE_NCL:
		return QNdCl_(LogNd,Vs);
		break;
	case TYPE_PCL:
		return QNaCl_(LogNd,Vs);
		break;
	case TYPE_NQ:
		return QNdQ_(LogNd,Vs);
		break;
	case TYPE_PQ:
		return QNaQ_(LogNd,Vs);
		break;
	}
	return 0.f;
}

float CInterpGaAs::Cp(float Vs, float LogNd, int type)
{
	float Nd,Na;
	if (type==TYPE_NCL || type==TYPE_NQ) { Nd = pow(10.,LogNd); Na = 0.; }
	else { Nd = 0.; Na = pow(10.,LogNd); }
	return FindSemiconductorCP(1.424f/2,-1.424f/2,1e19f,5e18f,Nd,Na,(float)(1/KT300));
}

static CInterpGaAs InterpGaAs;

CGaAsMISInterp::CGaAsMISInterp(int atype, float aCox, float aLogNd, float Vfb) :
CMISInterpBase(&InterpGaAs,atype,aCox,aLogNd,Vfb)
{}

CGaAsMISitInterp::CGaAsMISitInterp(CQitArray* aQit, int atype, float aCox, float aLogNd, float Vfb) :
CMISitInterpBase(aQit, &InterpGaAs,atype,aCox,aLogNd,Vfb)
{}







