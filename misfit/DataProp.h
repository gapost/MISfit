#if !defined(AFX_DATAPROP_H__FBF008FF_D178_4728_AEF1_2E1963304C74__INCLUDED_)
#define AFX_DATAPROP_H__FBF008FF_D178_4728_AEF1_2E1963304C74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataProp.h : header file
//

#include "MISProp.h"

/////////////////////////////////////////////////////////////////////////////
// CDataProp dialog

class CDataProp : public CMISProp
{
	DECLARE_DYNCREATE(CDataProp)

// Construction
public:
	CDataProp(CMISData* aMISData = NULL);
	~CDataProp();

// Dialog Data
	//{{AFX_DATA(CDataProp)
	enum { IDD = IDD_DATA_PROP };
	CListBox	lbData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDataProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDataProp)
	afx_msg void OnLoad();
	afx_msg void OnUnload();
	afx_msg void OnNewFreq();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAPROP_H__FBF008FF_D178_4728_AEF1_2E1963304C74__INCLUDED_)
