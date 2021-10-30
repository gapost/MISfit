// misfitDlg.h : header file
//

#if !defined(AFX_MISFITDLG_H__19D88681_C3F9_4A53_B868_73241A91BEE7__INCLUDED_)
#define AFX_MISFITDLG_H__19D88681_C3F9_4A53_B868_73241A91BEE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMisfitDlg dialog

class CMisfitDlg : public CDialog
{
// Construction
public:
	CMisfitDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMisfitDlg)
	enum { IDD = IDD_MISFIT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMisfitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMisfitDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MISFITDLG_H__19D88681_C3F9_4A53_B868_73241A91BEE7__INCLUDED_)
