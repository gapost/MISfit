#if !defined(AFX_MISDLG_H__2F88F380_8B8F_40D1_97D3_37FDCF2516D1__INCLUDED_)
#define AFX_MISDLG_H__2F88F380_8B8F_40D1_97D3_37FDCF2516D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MISDlg.h : header file
//

#include "MISProp.h"
#include "TrapProp.h"
#include "DataProp.h"



/////////////////////////////////////////////////////////////////////////////
// CMISDlg

class CMISDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CMISDlg)

// Construction
public:
	CMISDlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMISDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CMISData Data;
	CMISProp BasicP,PlotP;
	CTrapProp TrapP;
	CDataProp DataP;
	CButton btCalc,btAbout,btSaveParams,btHelp;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMISDlg)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMISDlg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMISDlg)
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg void OnCalc();
	afx_msg void OnReplot();
	afx_msg void OnSavePar();
	afx_msg void OnHtmHlp();
	afx_msg void OnAbout();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MISDLG_H__2F88F380_8B8F_40D1_97D3_37FDCF2516D1__INCLUDED_)
