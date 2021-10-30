#if !defined(AFX_GRAPHDLG_H__A9B35A68_2DEE_425C_BF82_4748EB14B8B7__INCLUDED_)
#define AFX_GRAPHDLG_H__A9B35A68_2DEE_425C_BF82_4748EB14B8B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphDlg.h : header file
//

#include "GraphData.h"
#include "Resource.h"


/////////////////////////////////////////////////////////////////////////////
// CAxisDlg dialog

class CAxisDlg : public CDialog
{
// Construction
public:
	CAxisDlg(CAxis* pAxis, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAxisDlg)
	enum { IDD = IDD_AXIS_PROPS };
	CEdit	m_edtX1;
	CEdit	m_edtX0;
	BOOL	bAutoscale;
	int		nAxisBase;
	CString	strTitle;
	float	x0;
	float	x1;
	//}}AFX_DATA
	CAxis* m_pAxis;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAxisDlg)
	afx_msg void OnAutoscale();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CSeriesDlg dialog

class CSeriesDlg : public CDialog
{
// Construction
public:
	CSeriesDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSeriesDlg)
	enum { IDD = IDD_SERIES_PROPS };
	int		m_ntype;
	//}}AFX_DATA

	void SetType(int atype);
	int  GetType();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeriesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSeriesDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHDLG_H__A9B35A68_2DEE_425C_BF82_4748EB14B8B7__INCLUDED_)
