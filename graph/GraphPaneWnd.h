#if !defined(AFX_GRAPHPANEWND_H__2015FD23_E7B5_437A_93A2_0D3A21C697D4__INCLUDED_)
#define AFX_GRAPHPANEWND_H__2015FD23_E7B5_437A_93A2_0D3A21C697D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphPaneWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraphPaneWnd window

#include "OffScreen.h"
#include "GraphData.h"

class CGraphPaneWnd : public CWnd
{
// Construction
public:
	CGraphPaneWnd();

// Attributes
public:
	CGraphData* m_pData;
	BOOL m_bZoomTool, m_bZoomStarted;
	CPoint ptDown, ptLast;
	CToolTipCtrl m_ToolTip;

// Operations
public:
	BOOL Create(CGraphData* pData, const RECT& rect, CWnd* pParentWnd, UINT nID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphPaneWnd)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Zoom(CRect& rect);
	bool MenuCommand(UINT nIDCommand);
	void AppendMenuItems(CMenu* menu);
	CGraphDataItem* HitTest(CPoint& pt);
	void Update();
	virtual ~CGraphPaneWnd();

	// Generated message map functions
protected:
	COffScreen m_Offscreen;
	//{{AFX_MSG(CGraphPaneWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHPANEWND_H__2015FD23_E7B5_437A_93A2_0D3A21C697D4__INCLUDED_)
