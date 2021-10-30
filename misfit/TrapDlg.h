#if !defined(AFX_TRAPDLG_H__4C67A80A_9F56_48FE_AE4A_B1C02C7E7F62__INCLUDED_)
#define AFX_TRAPDLG_H__4C67A80A_9F56_48FE_AE4A_B1C02C7E7F62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrapDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrapDlg dialog

#include "gvdata.h"

class CTrapDlg : public CDialog
{
// Construction
public:
	void EnableCtrls();
	CTrapDlg(CTrapData* pData, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTrapDlg)
	enum { IDD = IDD_TRAP_EDIT };
	//}}AFX_DATA

	CTrapData TempData;
	CTrapData* pTrapData;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTrapDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeTrapMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAPDLG_H__4C67A80A_9F56_48FE_AE4A_B1C02C7E7F62__INCLUDED_)
