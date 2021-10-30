#include "stdafx.h"

#include "qit.h"
#include "FermiFunc.h"

#define SQRT_PI 1.7724538509055160272981674833411


CQit::CQit(float aDit, float aS, bool ais_el_trap, bool ais_n_type,
		   float Eg, float aNc, float aNv, float avth) :
Dit(aDit/L0/N0), is_el_trap(ais_el_trap), is_n_type(ais_n_type), 
Ec(Eg/2), Ev(-Eg/2), Nc((float)(aNc/N0)), Nv((float)(aNv/N0)),
beta(1.f), vth_s(avth*aS*N0), S(aS),
CAdptRungeKutta<float>(2), y(2),
Next(0)
{
	set_beta(1/(float)KT300);
}

void CQit::set_beta(float abeta) {
	Ec = Ec/beta*abeta; Ev = Ev/beta*abeta; 
	Nc = Nc/beta*abeta; Nv = Nv/beta*abeta;
	vth_s = vth_s*beta/abeta;
	beta = abeta;
}

void CQit::Nit(double Vs, double& N, double &dN) {
	vs_ = Vs;
	int_type = 0;
	DoInteg();
	N = y[0];
	dN = y[1];
}

cdouble CQit::dNit(double Vs, double omega) {
	vs_ = Vs;
	omega_ = omega;
	int_type = 1;
	DoInteg();
	return cdouble(y[0],y[1]);
}

	
void CQit::calc_nit_(float x, TNT::Array1D<float>& n) {
	float g = git(x);
	double f,df;
	if (is_el_trap) {
		FermiFunc(x+vs_,1,f,df);
		n[0] = -g*f;
		n[1] = -g*df;
	}
	else {
		FermiFunc(x+vs_,1,f,df);
		n[0] = g*(1-f);
		n[1] = -g*df;
	}
}

void CQit::calc_dnit_(float x, TNT::Array1D<float>& dnit) {
	double n,dn,p,dp,f,df;
	float g,t,A,v = x + vs_;
	
	g = git(x);

	
	if (is_el_trap) FermiFunc(v,2,f,df);
	else FermiFunc(v,4,f,df);
	FD1_2( -(Ec+vs_), n, dn);
	FD1_2( Ev+vs_, p, dp);
	n *= Nc; p *= Nv;
	dn *= (-Nc); dp *= Nv;

	if (is_n_type) dp = 0.;
	else dn = 0.;

	t = 1./(n/f + p/(1-f));
	A = (dn*(1-f)-dp*f)*t;
	t /= vth_s;
	t *= omega_;
	dnit[0] = -g*A/(1.f+t*t);
	dnit[1] = -dnit[0]*t;
	

	/*if (is_el_trap) 
	{
		FermiFunc(v,2,f,df);
		FD1_2( -(Ec+vs_), n, dn);
		n *= Nc;
		dn *= (-Nc); 

		if (!is_n_type) dn = 0.;

		t = 1./(n/f); // + p/(1-f));
		A = dn*(1-f)*t; //-dp*f)*t;
		t /= vth_s;
		t *= omega_;
		dnit[0] = -g*A/(1.f+t*t);
		dnit[1] = -dnit[0]*t;
	}
	else
	{
		FermiFunc(v,4,f,df);
		FD1_2( Ev+vs_, p, dp);
		p *= Nv;
		dp *= Nv;

		if (is_n_type) dp = 0.;

		t = 1./(p/(1-f));
		A = -dp*f*t;
		t /= vth_s;
		t *= omega_;
		dnit[0] = -g*A/(1.f+t*t);
		dnit[1] = -dnit[0]*t;	
	}*/
}


CQitArray::~CQitArray()
{
	CQit* pQit = First;
	while (pQit) {
		CQit* N = pQit;
		pQit = pQit->Next;
		delete N;
	}
}

void CQitArray::Nit(double Vs, double &N, double &dN)
{
	N = 0.; dN = 0.;
	CQit* pQit = First;
	while (pQit) {
		double n,dn;
		pQit->Nit(Vs,n,dn);
		N += n;
		dN += dn;
		pQit = pQit->Next;
	}
}

cdouble CQitArray::dNit(double Vs, double omega)
{
	cdouble dn(0.,0.);
	CQit* pQit = First;
	while (pQit) {
		dn += pQit->dNit(Vs,omega);
		pQit = pQit->Next;
	}
	return dn;
}

void CQitArray::set_beta(float abeta)
{
	beta = abeta;
	CQit* pQit = First;
	while (pQit) {
		pQit->set_beta(abeta);
		pQit = pQit->Next;
	}
}

void CQitArray::Add(CQit *pQit)
{
	assert(pQit!=0);
	
	if (First) {
		CQit* P = First;
		CQit* Last;
		while (P) {
			Last = P;
			P = P->Next;
		}
		Last->Next = pQit;
	}
	else First = pQit;
	
}
