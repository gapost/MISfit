#if !defined(AFX_AXISWND_H__1C6E718A_FD4D_4362_BB84_083C8DB4CCFE__INCLUDED_)
#define AFX_AXISWND_H__1C6E718A_FD4D_4362_BB84_083C8DB4CCFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxisWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxisWnd window

#include "GraphData.h"
#include "GraphPaneWnd.h"

class CAxisWnd : public CWnd
{
// Construction
public:
	CAxisWnd();

// Attributes
public:
	CGraphData* m_pData;

	CGraphPaneWnd m_wndPane;

	CRect rPane, rAxis[4], rTitle[4], rUpdate[4];
	
	CGraphDataItem* pPopupClient;

	COLORREF backclr;

// Operations
public:
	BOOL Create(CGraphData* pData, const RECT& rect, CWnd* pParentWnd, UINT nID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawHere(CDC* pDC, CRect& rect);
	void HitTest(CPoint& pt);
	void PopupMenu(CPoint& pt);
	void Layout(int cx, int cy);
	void Update();
	virtual ~CAxisWnd();

	// Generated message map functions
protected:
	COffScreen m_Offscreen;
	//{{AFX_MSG(CAxisWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg BOOL OnEraseBkgnd(CDC* pDC) { return 1; }
	BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISWND_H__1C6E718A_FD4D_4362_BB84_083C8DB4CCFE__INCLUDED_)
