// SchrodingerBands.h: interface for the CSiConductionBand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCHRODINGERBANDS_H__4059F7F6_FD77_4F40_B439_4DCED9434E38__INCLUDED_)
#define AFX_SCHRODINGERBANDS_H__4059F7F6_FD77_4F40_B439_4DCED9434E38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Schrodinger1DItem.h"

class CSiElectronLow : public CSchrodinger1DLayer  
{
public:
	CSiElectronLow(CGrid* agrid) :
	  //CSchrodinger1DLayer(am, aLz, 0.98, 0.19, 2, 1, 0.56)
	  CSchrodinger1DLayer(agrid, 0.98, 1.4825*0.19, 2, 1, 0.56)
	  {}
};

class CSiElectronHigh : public CSchrodinger1DLayer  
{
public:
	CSiElectronHigh(CGrid* agrid) :
	  //CSchrodinger1DLayer(am, aLz, 0.19, 0.432, 4, 1, 0.56)
	  CSchrodinger1DLayer(agrid, 0.19, 1.4825*0.432, 4, 1, 0.56)
	  {}
};

class CGeElectron : public CSchrodinger1DLayer  
{
public:
	CGeElectron(CGrid* agrid) :
	  CSchrodinger1DLayer(agrid, 0.12, 0.303, 4, 1, 0.33)
	  {}
};

class CGaAsElectron : public CSchrodinger1DLayer  
{
public:
	CGaAsElectron(CGrid* agrid) :
	  CSchrodinger1DLayer(agrid, 0.067, 0.067, 1, 1, 1.424/2)
	  {}
};

class CSiHeavyHole : public CSchrodinger1DLayer  
{
public:
	CSiHeavyHole(CGrid* agrid) :
	  //CSchrodinger1DLayer(am, aLz, 0.45, 0.45, 1, -1, -0.56)
	  CSchrodinger1DLayer(agrid, 0.45, 0.45, 1, -1, -0.56)
	  {}
};

class CSiLightHole : public CSchrodinger1DLayer  
{
public:
	CSiLightHole(CGrid* agrid) :
	  //CSchrodinger1DLayer(am, aLz, 0.14, 0.14, 1, -1, -0.56)
	  CSchrodinger1DLayer(agrid, 0.14, 0.14, 1, -1, -0.56)
	  {}
};

class CSiSplitoffHole : public CSchrodinger1DLayer  
{
public:
	CSiSplitoffHole(CGrid* agrid) :
	  CSchrodinger1DLayer(agrid, 0.29, 0.29, 1, -1, -0.56-0.044)
	  {}
};

class CGeHeavyHole : public CSchrodinger1DLayer  
{
public:
	CGeHeavyHole(CGrid* agrid) :
	  CSchrodinger1DLayer(agrid, 0.28, 0.28, 1, -1, -0.33)
	  {}
};

class CGeLightHole : public CSchrodinger1DLayer  
{
public:
	CGeLightHole(CGrid* agrid) :
	  CSchrodinger1DLayer(agrid, 0.044, 0.044, 1, -1, -0.33)
	  {}
};

class CGeSplitoffHole : public CSchrodinger1DLayer  
{
public:
	CGeSplitoffHole(CGrid* agrid) :
	  CSchrodinger1DLayer(agrid, 0.084, 0.084, 1, -1, -0.33-0.028)
	  {}
};

class CGaAsHeavyHole : public CSchrodinger1DLayer  
{
public:
	CGaAsHeavyHole(CGrid* agrid) :
	  CSchrodinger1DLayer(agrid, 0.45, 0.45, 1, -1, -1.424/2)
	  {}
};

class CGaAsLightHole : public CSchrodinger1DLayer  
{
public:
	CGaAsLightHole(CGrid* agrid) :
	  CSchrodinger1DLayer(agrid, 0.082, 0.082, 1, -1, -1.424/2)
	  {}
};

class CGaAsSplitoffHole : public CSchrodinger1DLayer  
{
public:
	CGaAsSplitoffHole(CGrid* agrid) :
	  CSchrodinger1DLayer(agrid, 0.154, 0.154, 1, -1, -1.424/2-0.34)
	  {}
};

class CSiO2Electron : public CSchrodinger1DLayer  
{
public:
	CSiO2Electron(CGrid* agrid, double aMox) :
	  CSchrodinger1DLayer(agrid, aMox, aMox, 1, 1, 3.1+0.56)
	  {}
};

class CSiO2Hole : public CSchrodinger1DLayer  
{
public:
	CSiO2Hole(CGrid* agrid, double aMox) :
	  CSchrodinger1DLayer(agrid, aMox, aMox, 1, -1, -3.1-0.56)
	  {}
};

class COxideElectron : public CSchrodinger1DLayer  
{
public:
	COxideElectron(CGrid* agrid, double aMox, double dEc) :
	  CSchrodinger1DLayer(agrid, aMox, aMox, 1, 1, dEc+0.56)
	  {}
};

class COxideHole : public CSchrodinger1DLayer  
{
public:
	COxideHole(CGrid* agrid, double aMox, double dEv) :
	  CSchrodinger1DLayer(agrid, aMox, aMox, 1, -1, -dEv-0.56)
	  {}
};

#endif // !defined(AFX_SCHRODINGERBANDS_H__4059F7F6_FD77_4F40_B439_4DCED9434E38__INCLUDED_)
