#if !defined(AFX_BASICPROP_H__DF02FDC7_2D7F_4B82_85BA_9621134C7C9C__INCLUDED_)
#define AFX_BASICPROP_H__DF02FDC7_2D7F_4B82_85BA_9621134C7C9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BasicProp.h : header file
//

#include "MISProp.h"

/////////////////////////////////////////////////////////////////////////////
// CBasicProp dialog

class CBasicProp : public CMISProp
{
	DECLARE_DYNCREATE(CBasicProp)

// Construction
public:
	CBasicProp(CMISData* aMISData=0);
	~CBasicProp();

// Dialog Data
	//{{AFX_DATA(CBasicProp)
	enum { IDD = IDD_BASIC_PROP };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBasicProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBasicProp)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASICPROP_H__DF02FDC7_2D7F_4B82_85BA_9621134C7C9C__INCLUDED_)
