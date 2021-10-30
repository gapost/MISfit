#if !defined(AFX_MISPROP_H__C51E1157_F641_4046_9D25_CC111C134B85__INCLUDED_)
#define AFX_MISPROP_H__C51E1157_F641_4046_9D25_CC111C134B85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MISProp.h : header file
//

#include "GVdata.h"

typedef CArray<CTrapData,CTrapData&> CTrapArray;

class CMISData : public CDataArray 
{
// Construction
public:
	void SavePar(CString& S);
	void UpdatePlot();
	void DoCalc();
	void DoDataExchange(CDataExchange* pDX);

	float eGap()
	{
		switch(nSubMaterial)
		{
		case 0: // Si
			return (float)SI_EG;
		case 1: // Ge
			return (float)GE_EG;
		case 2:
			return (float)GAAS_EG;
		default:
			return 0;
		}
	}

	CMISData() :	// standard constructor
	Cox(1.f),Nd(1e16f),bQuant(FALSE),nSubtype(0),nSubMaterial(0),
	Vfb(0.f),Gp(0.f),Gs(1e6f),Vts(0.f),
	Graph("MISFIT plot",2,1,500,700)
	{}
	virtual ~CMISData() {}

// Dialog Data
	float	Cox;
	float	Nd;
	float	EOT;
	BOOL	bQuant;
	int		nSubtype;
	int		nSubMaterial;
	float	Vfb;
	float	Gp;
	float	Gs;
	float	Vts;

	CPlotOptions PlotOptions;

// Traps
	CTrapArray Traps;
// PlotWindow
	CGraphFrame Graph;
};

/////////////////////////////////////////////////////////////////////////////
// CMISProp dialog

class CMISProp : public CPropertyPage
{
	DECLARE_DYNCREATE(CMISProp)

// Construction
public:
	CMISProp(UINT uID = 0,CMISData* aMISData = NULL, BOOL isExec = FALSE);
	~CMISProp();

// Dialog Data
	//{{AFX_DATA(CMISProp)
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

	CMISData* pMISData;
	BOOL bExecPage;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMISProp)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMISProp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MISPROP_H__C51E1157_F641_4046_9D25_CC111C134B85__INCLUDED_)
