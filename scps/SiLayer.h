#ifndef SILAYER_H
#define SILAYER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DielectricLayer.h"

class CClassicalSiLayer : public CSemiconductorLayer {
public:
	CClassicalSiLayer(CGrid* agrid, double aNd, double aNa, 
		bool afermi = 1) : 
	CSemiconductorLayer(agrid, 
		11.8, 0.56, -0.56, 0.04, 0.04, 2.8e19, 1.04e19, 
		aNd, aNa, afermi) 
	{}
};

class CClassicalGeLayer : public CSemiconductorLayer {
public:
	CClassicalGeLayer(CGrid* agrid, double aNd, double aNa, 
		bool afermi = 1) : 
	CSemiconductorLayer(agrid, 
		16, 0.33, -0.33, 0.01, 0.011, 1.0e19 , 5.0e18 , 
		aNd, aNa, afermi) 
	{}
};

class CClassicalGaAsLayer : public CSemiconductorLayer {
public:
	CClassicalGaAsLayer(CGrid* agrid, double aNd, double aNa, 
		bool afermi = 1) : 
	CSemiconductorLayer(agrid, 
		13.1, 1.424/2, -1.424/2, 0.0058, 0.026, 4.7e17 , 7.0e18 , 
		aNd, aNa, afermi) 
	{}
};

class CQuantumSiLayer : public CQuantumSemiconductorLayer {
public:
	CQuantumSiLayer(CGrid* agrid, double aNd, double aNa) : 
	CQuantumSemiconductorLayer(agrid, 
		11.8, 0.56, -0.56, 0.04, 0.04, 2.8e19, 1.04e19, 
		aNd, aNa) 
	{}
};

class CQuantumGeLayer : public CQuantumSemiconductorLayer {
public:
	CQuantumGeLayer(CGrid* agrid, double aNd, double aNa) : 
	CQuantumSemiconductorLayer(agrid, 
		16, 0.33, -0.33, 0.01, 0.011, 1.0e19 , 5.0e18 , 
		aNd, aNa) 
	{}
};

class CQuantumGaAsLayer : public CQuantumSemiconductorLayer {
public:
	CQuantumGaAsLayer(CGrid* agrid, double aNd, double aNa) : 
	CQuantumSemiconductorLayer(agrid, 
		13.1, 1.424/2, -1.424/2, 0.0058, 0.026, 4.7e17 , 7.0e18 , 
		aNd, aNa) 
	{}
};

#endif 
