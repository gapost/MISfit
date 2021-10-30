#ifndef _QIT1_H
#define _QIT1_H

#include "..\scps\constants.h"

#define NNC (2.8e19/N0)
#define NNV (1.04e19/N0)


class CQit4 : protected CAdptRungeKutta {
public:
	CQit4(double aDit, double S, bool ais_el_trap) :
	  Dit(aDit/L0/N0), is_el_trap(ais_el_trap), 
		  Ec(0.56f), Ev(-0.56f), Nc((float)(2.8e19f/N0)), Nv((float)(1.04e19/N0)),
		  beta(1.f), vth_s(VTH*S*N0), 
		  CAdptRungeKutta(2), y(2) 
	  {
		  set_beta(1/(float)KT300);
	  }
	  
	  virtual void set_beta(float abeta) {
		  Ec = Ec/beta*abeta; Ev = Ev/beta*abeta; 
		  Nc = Nc/beta*abeta; Nv = Nv/beta*abeta;
		  vth_s = vth_s*beta/abeta;
		  beta = abeta;
	  }
	  virtual float git(float x) { 
		  /*if (is_el_trap) {
			  if (x>0) return Dit; else return 0.;
		  }
		  else {
			  if (x<0) return Dit; else return 0.;
		  }*/
		  return Dit;
	  }
	  void SetDit(double aDit) { Dit = aDit*beta/L0/N0; }
	  void SetS(double aS) { vth_s = VTH*aS*N0*beta; }
	  
	  void Nit(double Vs, double& N, double &dN) {
		  vs_ = Vs;
		  int_type = 0;
		  y[0] = 0.f;
		  y[1] = 0.f;
		  reset();
		  if (is_el_trap) Integrate(y,0.f,Ec,1e-5f,Ec/100,0.f);
		  else Integrate(y,Ev,0.f,1e-5f,Ev/100,0.f);
		  N = y[0];
		  dN = y[1];
	  }
	  void Qit(double Vs, double& Q, double &dQ) {
		  Nit(Vs,Q,dQ);
		  double conv = N0*L0/beta*ELECTRON_CHARGE;
		  Q *= conv;
		  dQ *= conv;
	  }
	  cdouble dNit(double Vs, double omega) {
		  vs_ = Vs;
		  omega_ = omega;
		  int_type = 1;
		  y[0] = 0.f;
		  y[1] = 0.f;
		  reset();
		  if (is_el_trap) Integrate(y,0.f,Ec,1e-5f,Ec/100,0.f);
		  else Integrate(y,Ev,0.f,1e-5f,Ev/100,0.f);
		  return cdouble(y[0],y[1]);
	  }
	  cdouble Cit(double Vs, double omega) {
		  double conv = N0*L0*ELECTRON_CHARGE;
		  return dNit(Vs,omega)*conv;
	  }
	  
	  float CP_(float Nd, float Na)
	  {
		  //double Nd,Na;
		  //if (type==TYPE_NCL || type==TYPE_NQ) { Nd = pow(10.,LogNd); Na = 0.; }
		  //else { Nd = 0.; Na = pow(10.,LogNd); }
		  
		  // initial estimate
		  Nd /= N0;
		  Na /= N0;
		  double v = (Nd-Na)/sqrt(NNV*NNC)/exp(-(Ec - Ev)/2)/2;
		  v = asinh(v);
		  v = 0.5*log(NNV/NNC) + v;
		  v = (Ec - Ev)/2 - Ec - v;
		  
		  // refine
		  double q,dq;
		  do {
			  {
				  double ec = Ec+v, ev = Ev+v;
				  double n,dn,p,dp;
				  
				  ImpG(1,ec-beta*0.04,n,dn);
				  ImpG(1,-ev-beta*0.04,p,dp,4);
				  q = Nd*n - Na*p;
				  dq = Nd*dn + Na*dp;
				  
				  FermiIntegral(1,-ec,n,dn);
				  q -= NNC*n; 
				  dq += NNC*dn; 
				  
				  FermiIntegral(1,ev,p,dp);
				  q += NNV*p; 
				  dq += NNV*dp; 
				  
				  q *= beta;
				  dq *= beta;
			  } 
			  v -= q/dq;
		  } while (fabs(q)>1e-6);
		  
		  return v/beta;
	  }	  
	  
protected:
	virtual void odefunc(float x, const TNT::Array1D<float>& y, TNT::Array1D<float>& dydx) {
		if (int_type==0) calc_nit_(x,dydx);
		else calc_dnit_(x,dydx);
	}
	void calc_nit_(float x, TNT::Array1D<float>& n) {
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
	void calc_dnit_(float x, TNT::Array1D<float>& dnit) {
		double n,dn,p,dp,f,df;
		float g,t,A,v = x + vs_;
		
		g = git(x);
		if (is_el_trap) FermiFunc(v,2,f,df);
		else FermiFunc(v,4,f,df);
		FD1_2( -(Ec+vs_), n, dn);
		FD1_2( Ev+vs_, p, dp);
		n *= Nc; p *= Nv;
		dn *= (-Nc); dp *= Nv;
		if (is_el_trap) dp = 0.;
		else dn = 0.;
		t = 1./(n/f + p/(1-f));
		A = (dn*(1-f)-dp*f)*t;
		t /= vth_s;
		t *= omega_;
		dnit[0] = -g*A/(1.f+t*t);
		dnit[1] = -dnit[0]*t;
	}
	float vs_, omega_;
	int int_type;
	float Dit,Ec,Ev,Nc,Nv,beta,vth_s;
	bool is_el_trap;
	TNT::Array1D<float> y;
};

class CQitGauss4 : public CQit4 {
public:
	CQitGauss4(double aDit, double aEo, double aDE, double S, bool ais_el_trap) :
	  CQit4(aDit, S, ais_el_trap) , Eo(aEo), DE(aDE)
	  {}
	  virtual void set_beta(double abeta) {
		  Eo = Eo/beta*abeta; DE = DE/beta*abeta; 
		  CQit4::set_beta(abeta);
	  }
	  virtual float git(float x) { 
		  float g = (x - Eo)/DE;
		  return exp(-g*g)*Dit;
	  }
protected:
	  float Eo,DE;
};

#endif