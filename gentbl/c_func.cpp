#include "stdafx.h"

#define VMAX 2.5
#define NMIN 13
#define NMAX 18
#define NV 151
#define NN 21

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


#include "misint.h"

CSpliner2D<float> CMISInterp::QNdCl_(NN,NV,logNd,V,&Q_Nd_Cl[0][0]);
CSpliner2D<float> CMISInterp::CNdCl_(NN,NV,logNd,V,&C_Nd_Cl[0][0]);
CSpliner2D<float> CMISInterp::QNaCl_(NN,NV,logNd,V,&Q_Na_Cl[0][0]);
CSpliner2D<float> CMISInterp::CNaCl_(NN,NV,logNd,V,&C_Na_Cl[0][0]);

CSpliner2D<float> CMISInterp::QNdQ_(NN,NV,logNd,V,&Q_Nd_Q[0][0]);
CSpliner2D<float> CMISInterp::CNdQ_(NN,NV,logNd,V,&C_Nd_Q[0][0]);
CSpliner2D<float> CMISInterp::QNaQ_(NN,NV,logNd,V,&Q_Na_Q[0][0]);
CSpliner2D<float> CMISInterp::CNaQ_(NN,NV,logNd,V,&C_Na_Q[0][0]);

CMISInterp::CMISInterp(int atype, float aCox, float aLogNd) :
Cox(aCox), LogNd(aLogNd), Vfb(0.f), type(atype)
{}

float CMISInterp::Qs_(float Vs)
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

float CMISInterp::Vs_(float V)
{
	float x1 , x2;
	//if (V==0.f) return 0.f;
	//else if (V>0.f) x2 = max(V,VMAX); else x2 = min(V,-VMAX);
	if (V>0.f) { x2 = VMAX; x1 = -VMAX; }
	else { x2 = -VMAX; x1 = VMAX; }
	
	
	int j,JMAX=40;
	float dx,f,fmid,xmid,rtb;
	
	
	f = (V-x1)*Cox;
	fmid = (V-x2)*Cox - Qs_(x2);
	if (f*fmid >= 0.0) {
		//assert(1==0);
		return VMAX * V / fabs(V);
	}
	rtb = f < 0.0 ? (dx=x2-x1,x1) : (dx=x1-x2,x2); // Orient the search so that f>0 lies at x+dx.
	for (j=1;j<=JMAX;j++) {
		xmid=rtb+(dx *= 0.5);
		fmid=(V-xmid)*Cox - Qs_(xmid); //Bisection loop.
		if (fmid <= 0.0) rtb=xmid;
		if (fabs(dx) < 1e-3 || fmid == 0.0) return rtb;
	}
	//assert(1==0);
	return VMAX * V / fabs(V); //Never get here.
}

float CMISInterp::Ctot(float V, float& Vs)
{
	Vs = Vs_(- (V-Vfb));
	float Cs = Cs_(Vs);
	return Cs*Cox/(Cs+Cox);
}

float CMISInterp::Cs_(float Vs)
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


float CMISitInterp::Qs_(float Vs)
{
	double Qi,dQi;
	Qit->Qit((Vs+cp_)/KT300,Qi,dQi);
	return CMISInterp::Qs_(Vs) + Qi;
}

cdouble CMISitInterp::Ctot(float V, float omega, float& Vs)
{
	Vs = Vs_(- (V-Vfb));
	cdouble Cit = Qit->Cit((double)(Vs+cp_)/KT300,(double)omega);
	Cit += Cs_(Vs);
	return Cit*(double)Cox/(Cit+(double)Cox);
}

void CMISitInterp::CP_()
{
	float Nd,Na;
	if (type==TYPE_NCL || type==TYPE_NQ) { Nd = pow(10.,LogNd); Na = 0.; }
	else { Nd = 0.; Na = pow(10.,LogNd); }

	cp_ = Qit->CP_(Nd,Na);
}






