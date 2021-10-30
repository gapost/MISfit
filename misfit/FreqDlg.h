#if !defined(AFX_FREQDLG_H__F4A336D5_767F_469B_8434_108DA7C5AF79__INCLUDED_)
#define AFX_FREQDLG_H__F4A336D5_767F_469B_8434_108DA7C5AF79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FreqDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFreqDlg dialog

class CFreqDlg : public CDialog
{
// Construction
public:
	CFreqDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFreqDlg)
	enum { IDD = IDD_FREQ_DLG };
	float	Freq;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFreqDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFreqDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FREQDLG_H__F4A336D5_767F_469B_8434_108DA7C5AF79__INCLUDED_)
