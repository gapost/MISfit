// Grid.h: interface for the CGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRID_H__ACF69CC3_E86A_45B1_8DE7_7A68AB413BAA__INCLUDED_)
#define AFX_GRID_H__ACF69CC3_E86A_45B1_8DE7_7A68AB413BAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGrid  
{
public:
	CGrid(int am, double aL) : m(am-1), L(aL) { assert(m>0); assert(L>0.); }
	virtual ~CGrid() {}
	virtual MakeGrid(double* px) = 0;
	double get_L() { return L; }
	int get_m() { return m; }
protected:
	int m;
	double L;
};

class CLinearGrid : public CGrid
{
public:
	CLinearGrid(int am, double aL) : CGrid(am,aL) {}
	virtual MakeGrid(double* px) 
	{
		double dx = L/(m+1);

		double* xend = px + m + 1;

		for(; px!=xend; px++) *px = dx + *(px-1);
	}
};

class CLogGrid : public CGrid
{
public:
	CLogGrid(int am, double aL, double adxo) : CGrid(am,aL), dxo(adxo) { assert(dxo>0.); }
	virtual MakeGrid(double* px) 
	{
		double b = dxo, x_1=*(px-1), D=pow(L/dxo,1./m);
		double dx = L/(m+1);
		
		for(int i=0; i<=m; i++) {
			*px = x_1 + b;
			px++;
			b *= D;
		}
	}
protected:
	double dxo;
};


/*
void CPoisson1DLayer::MakeUniformGrid()
{
	double dx = L/(m+1);

	double* xend = x + m + 1;

	for(double* p = x; p!=xend; p++) *p = dx + *(p-1);
}

if (solidgrid==0) R.Init(m+1);
void CPoisson1DLayer::RefineGrid(double delta)
{
	if (solidgrid) return;

	if(R.Refine(x-1,v-1,delta)) {
		R.Convert(v-1);
		R.Convert(cp-1);
	}
}
*/


#endif // !defined(AFX_GRID_H__ACF69CC3_E86A_45B1_8DE7_7A68AB413BAA__INCLUDED_)
