// misfit.h : main header file for the MISFIT application
//

#if !defined(AFX_MISFIT_H__C93F04FC_23E6_4132_AFCD_71308EB1202D__INCLUDED_)
#define AFX_MISFIT_H__C93F04FC_23E6_4132_AFCD_71308EB1202D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMisfitApp:
// See misfit.cpp for the implementation of this class
//

class CMisfitApp : public CWinApp
{
public:
	CMisfitApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMisfitApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMisfitApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MISFIT_H__C93F04FC_23E6_4132_AFCD_71308EB1202D__INCLUDED_)
