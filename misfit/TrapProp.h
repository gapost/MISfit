#if !defined(AFX_TRAPPROP_H__93EA0E2A_B92D_47D7_A397_487837DAD9F1__INCLUDED_)
#define AFX_TRAPPROP_H__93EA0E2A_B92D_47D7_A397_487837DAD9F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrapProp.h : header file
//

#include "MISProp.h"

/////////////////////////////////////////////////////////////////////////////
// CTrapProp dialog

class CTrapProp : public CMISProp
{
	DECLARE_DYNCREATE(CTrapProp)

// Construction
public:
	CTrapProp(CMISData* aMISData = NULL);
	~CTrapProp();

// Dialog Data
	//{{AFX_DATA(CTrapProp)
	enum { IDD = IDD_TRAP_PROP };
	CListBox	lbTraps;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTrapProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTrapProp)
	afx_msg void OnAddTrap();
	afx_msg void OnDelTrap();
	afx_msg void OnEditTrap();
	afx_msg void OnDblclkTrapList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAPPROP_H__93EA0E2A_B92D_47D7_A397_487837DAD9F1__INCLUDED_)
