#ifndef QIT_H
#define QIT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "constants.h"

#define VTH 1e7
#define CONV (N0*L0*ELECTRON_CHARGE)

class CQit : protected CAdptRungeKutta<float> {
public:
	CQit(float aDit, float aS, bool ais_el_trap, bool ais_n_type,
		   float Eg, float aNc, float aNv, float avth);
	  
	  virtual void set_beta(float abeta); 
	  virtual float git(float x) { return Dit; }
	  void SetDit(double aDit) { Dit = aDit/L0/N0; }
	  void SetS(double aS) { vth_s = (vth_s/S)*aS; }
	  
	  void Nit(double Vs, double& N, double &dN); 
	  void Qit(double Vs, double& Q, double &dQ) {
		  Nit(Vs,Q,dQ);
		  Q = Q*CONV/beta; 
		  dQ = dQ*CONV/beta;
	  }

	  cdouble dNit(double Vs, double omega); 
	  cdouble Cit(double Vs, double omega) { return dNit(Vs,omega)*CONV; }
	  	  
protected:
	virtual void odefunc(float x, const TNT::Array1D<float>& y, TNT::Array1D<float>& dydx) {
		if (int_type==0) calc_nit_(x,dydx);
		else calc_dnit_(x,dydx);
	}
	virtual void DoInteg() {
		y[0] = 0.f; y[1] = 0.f; reset();
		//Integrate(y,Ev,Ec,1e-5f,Ec/100,0.f);
		if (is_el_trap) Integrate(y,0.f,Ec,1e-5f,Ec/100,0.f);
		else Integrate(y,Ev,0.f,1e-5f,Ev/100,0.f);
	}
	void calc_nit_(float x, TNT::Array1D<float>& n);
	void calc_dnit_(float x, TNT::Array1D<float>& dnit);
	float vs_, omega_;
	int int_type;
	float Dit,Ec,Ev,Nc,Nv,beta,vth_s,S;
	bool is_el_trap, is_n_type;
	TNT::Array1D<float> y;
	CQit* Next;

	friend class CQitArray;
};

class CQitArray {
public:
	void Add(CQit* pQit);
	CQitArray() : First(0), beta((float)(1/KT300)) {}
	~CQitArray();
	
	void set_beta(float abeta);

	void Nit(double Vs, double& N, double& dN);
	void Qit(double Vs, double& Q, double &dQ) {
		Nit(Vs,Q,dQ);
		Q = Q*CONV/beta; 
		dQ = dQ*CONV/beta;
	}

	cdouble dNit(double Vs, double omega);
	cdouble Cit(double Vs, double omega) { return dNit(Vs,omega)*CONV; }
	
protected:
	CQit* First;
	float beta;
};


class CQitMatrix : public CQit {
public:
	CQitMatrix(int an, float* aDitArray, float* aEoArray,
		float S, bool ais_el_trap, bool ais_n_type,
		   float Eg, float aNc, float aNv, float avth) :
	  CQit(0., S, ais_el_trap, ais_n_type,Eg,aNc,aNv,avth), 
		  DitArray(an), EoArray(an), n(an)
	  {
		  for(int i=0; i<n; i++) {
			  DitArray[i] = log(aDitArray[i]/N0/L0);
			  EoArray[i] = aEoArray[i]*beta;
		  }
	  }
	  virtual void set_beta(float abeta) {
		  for(int i=0; i<n; i++) {
			  EoArray[i] = EoArray[i]/beta*abeta;
		  }
		  CQit::set_beta(abeta);
	  }
	  virtual float git(float x) {
		  int k = 1;
		  while (x>EoArray[k] && k<n-1) k++;
		  float g =  (DitArray[k] - DitArray[k-1])/(EoArray[k] - EoArray[k-1])*(x-EoArray[k-1]) + DitArray[k-1];
		  return exp(g);
	  }
protected:
	TNT::Array1D<float> DitArray,EoArray;
	int n;
	virtual void DoInteg() {
		y[0] = 0.f; y[1] = 0.f; reset();
		Integrate(y,EoArray[0],EoArray[n-1],1e-5f,Ec/100,0.f);
	}
};

class CQitGauss : public CQit {
public:
	CQitGauss(float aDit, float aEo, float aDE, float aS, bool ais_el_trap, bool ais_n_type,
		   float Eg, float aNc, float aNv, float avth) :
	  CQit(aDit, aS, ais_el_trap, ais_n_type, Eg, aNc, aNv, avth)
	  {
		  Eo = beta*aEo;
		  DE = beta*aDE*1.20112;
	  }
	  virtual void set_beta(float abeta) {
		  Eo = abeta*Eo/beta;
		  DE = abeta*DE/beta;
		  CQit::set_beta(abeta);
	  }
	  virtual float git(float x) {
		  x = (x-Eo)/DE;
		  return Dit*exp(-x*x);
	  }
protected:
	float Eo,DE;
};

class CQitLorentz : public CQit {
public:
	CQitLorentz(float aDit, float aEo, float aDE, float aS, bool ais_el_trap, bool ais_n_type,
		   float Eg, float aNc, float aNv, float avth) :
	  CQit(aDit, aS, ais_el_trap, ais_n_type, Eg, aNc, aNv, avth)
	  {
		  Eo = beta*aEo;
		  DE = beta*aDE;
	  }
	  virtual void set_beta(float abeta) {
		  Eo = abeta*Eo/beta;
		  DE = abeta*DE/beta;
		  CQit::set_beta(abeta);
	  }
	  virtual float git(float x) {
		  x = (x-Eo)/DE;
		  return Dit/(1.f+x*x);
	  }
protected:
	float Eo,DE;
};
#endif // !defined(AFX_POISSON1D_H__6933662D_63CF_41CE_977C_41A8B0661205__INCLUDED_)
