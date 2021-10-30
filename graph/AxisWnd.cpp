// AxisWnd.cpp : implementation file
//

#include "stdafx.h"
#include "AxisWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxisWnd

CAxisWnd::CAxisWnd() : pPopupClient(NULL), backclr(BCKGND_STD)
{
}

CAxisWnd::~CAxisWnd()
{
}


BEGIN_MESSAGE_MAP(CAxisWnd, CWnd)
	//{{AFX_MSG_MAP(CAxisWnd)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAxisWnd message handlers

BOOL CAxisWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::PreCreateWindow(cs);
}

BOOL CAxisWnd::Create(CGraphData* pData, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	ASSERT(pData!=NULL);
	m_pData = pData;

	CRect crect(&rect);
	m_Offscreen.Create(crect.Width(),crect.Height());

	BOOL ret = CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID);

	if (!ret) return ret;

	Layout(crect.Width(),crect.Height());

	ret = m_wndPane.Create(m_pData, rPane, this, 12);

	EnableToolTips(TRUE);

	return ret;
}

void CAxisWnd::Update()
{
	CDC* pDC = m_Offscreen.GetDC();
	
	if (pDC==NULL) return;
	
	
	int W = m_Offscreen.Width(), H = m_Offscreen.Height();
	pDC->FillSolidRect(0,0,W,H,backclr); 
	
	CSeriesArray& Series = m_pData->Series;
	CAxisArray& Axes = m_pData->Axes;
	int naxes = Axes.GetSize();
	int nseries = Series.GetSize();
	int i;
	
	// Rescan Data
	for(i=0; i<naxes; i++) Axes[i]->ResetLimits();
	for(i=0; i<nseries; i++) Series[i]->ScanData();
	
	// Draw Titles
	for(i=0; i<naxes; i++) {
		CAxis* p = Axes[i];
		p->SetFontHeight(W,H);
		p->DrawTitle(pDC, rTitle);
		p->FixDiv();
		p->DrawLabels(pDC, rAxis);
	}
	
	m_wndPane.Update();
	
	Invalidate();
}

void CAxisWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	m_Offscreen.Resize(cx,cy);

	Layout(cx,cy);

	if (::IsWindow(m_wndPane.m_hWnd) ) m_wndPane.MoveWindow(&rPane,FALSE);

	Update();
	
}

void CAxisWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	for(int i=0; i<4; i++)
		m_Offscreen.DrawHere(&dc,rUpdate[i]);
	
	// Do not call CWnd::OnPaint() for painting messages
}

void CAxisWnd::Layout(int cx, int cy)
{
	int w = cx*0.1, h=cy*0.1, 
		w1 = w*0.4, h1 = h*0.4,
		w2 = w*0.3, h2 = h*0.3;

	rPane.SetRect(w,h,cx-w,cy-h);

	// bottom
	rTitle[0].SetRect(w,cy-h1,cx-w,cy);
	rAxis[0].SetRect(w,cy-h,cx-w,cy-h1);
	rUpdate[0].SetRect(0,cy-h,cx-w,cy);

	// left
	rTitle[1].SetRect(0,h,w2,cy-h);
	rAxis[1].SetRect(w2,h,w,cy-h);
	rUpdate[1].SetRect(0,0,w,cy-h);

	// top
	rTitle[2].SetRect(w,0,cx-w,h1);
	rAxis[2].SetRect(w,h1,cx-w,h);
	rUpdate[2].SetRect(w,0,cx,h);

	// right
	rTitle[3].SetRect(cx-w2,h,cx,cy-h);
	rAxis[3].SetRect(cx-w,h,cx-w2,cy-h);
	rUpdate[3].SetRect(cx-w,h,cx,cy);
}

void CAxisWnd::PopupMenu(CPoint &pt)
{
	CMenu menu;
	if (!menu.CreatePopupMenu()) return;

	if (rPane.PtInRect(pt)) {
		m_wndPane.AppendMenuItems(&menu);
		CPoint pt1(pt.x-rPane.left,pt.y-rPane.top);
		if ( (pPopupClient=m_wndPane.HitTest(pt1))!=NULL ) 
			pPopupClient->AppendMenuItems(&menu);
	}
	else {
		
		HitTest(pt);
		if (pPopupClient) pPopupClient->AppendMenuItems(&menu);
	}

	ClientToScreen(&pt);
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,pt.x,pt.y,this);
}

BOOL CAxisWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	UINT id = LOWORD(wParam);
	UINT code = HIWORD(wParam);
	if (id>GRAPH_ITEM_POPUP_FIRST && id <GRAPH_ITEM_POPUP_LAST) {
		// Give a chance to graph items to process the command
		if (pPopupClient) {
			if (pPopupClient->MenuCommand(id)) {
				Update();
				pPopupClient=NULL;
				return TRUE;
			}
		}

		if (m_wndPane.MenuCommand(id)) {
				Update();
				return TRUE;
		}
	}

	return CWnd::OnCommand(wParam, lParam);
}

void CAxisWnd::HitTest(CPoint &pt)
{
	CAxisArray& Axes = m_pData->Axes;
	int n = Axes.GetSize();
	for(int i=0; i<n; i++) {
		if (rTitle[i].PtInRect(pt) || rAxis[i].PtInRect(pt))
			pPopupClient = Axes[i];
	}

}

BOOL CAxisWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::OnNotify(wParam, lParam, pResult);
}


BOOL CAxisWnd::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID =pNMHDR->idFrom;
/*    if (pTTT->uFlags & TTF_IDISHWND)
    {
        // idFrom is actually the HWND of the tool
        nID = ::GetDlgCtrlID((HWND)nID);
        if(nID)
        {
            pTTT->lpszText = MAKEINTRESOURCE(nID);
            pTTT->hinst = AfxGetResourceHandle();
            return(TRUE);
        }
    }*/
    return(FALSE);
}


void CAxisWnd::DrawHere(CDC *pDC, CRect &rect)
{
	for(int i=0; i<4; i++)
		m_Offscreen.DrawHere(pDC,rUpdate[i], rect.TopLeft());

}
