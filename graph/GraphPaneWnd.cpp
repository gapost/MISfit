// GraphPaneWnd.cpp : implementation file
//

#include "stdafx.h"
#include "GraphPaneWnd.h"
#include "AxisWnd.h"

#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphPaneWnd

CGraphPaneWnd::CGraphPaneWnd() : 
m_bZoomTool(FALSE), m_bZoomStarted(FALSE)
{
}

CGraphPaneWnd::~CGraphPaneWnd()
{
}


BEGIN_MESSAGE_MAP(CGraphPaneWnd, CWnd)
	//{{AFX_MSG_MAP(CGraphPaneWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGraphPaneWnd message handlers

BOOL CGraphPaneWnd::OnEraseBkgnd(CDC* pDC) 
{
	//CRect rect;
	//GetClientRect(&rect);
	//pDC->FillSolidRect(&rect,RGB(255,255,255));

	return 1;
}

BOOL CGraphPaneWnd::Create(CGraphData* pData, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	ASSERT(pData!=NULL);
	m_pData = pData;

	CRect crect(&rect);
	m_Offscreen.Create(crect.Width(),crect.Height());
	
	BOOL ret = CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID);

	if (!ret) return ret;

	if (m_ToolTip.Create(this)) {
		EnableToolTips(TRUE);
		m_ToolTip.AddTool(this);
		m_ToolTip.Activate(TRUE);
	}

	return ret;
}


void CGraphPaneWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	dc.GetClipBox(&rect);
	m_Offscreen.DrawHere(&dc,rect);

	// Do not call CWnd::OnPaint() for painting messages
}

void CGraphPaneWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_Offscreen.Resize(cx,cy);

	//m_pData->SetSize(cx,cy);

	//Update();
	
}

void CGraphPaneWnd::Update()
{
	CDC* pDC = m_Offscreen.GetDC();
	if (pDC==NULL) return;

	CRect rect(0,0,m_Offscreen.Width(),m_Offscreen.Height());

	// Draw Bounding Rect
	CGdiObject *pOldBrush = pDC->SelectStockObject(WHITE_BRUSH);
	pDC->Rectangle(&rect);
	pDC->SelectObject(pOldBrush);

	rect.right -= 1;
	rect.bottom -= 1;

	// Draw Tics
	{
		CAxisArray& Axes = m_pData->Axes;
		int n = Axes.GetSize();
		for(int i=0; i<n; i++) Axes[i]->DrawTicks(pDC,rect);
	}

	// Draw Data
	{
		CSeriesArray& Series = m_pData->Series;
		int n = Series.GetSize();
		for(int i=0; i<n; i++) Series[i]->Draw(pDC,rect);
	}

	Invalidate();
}

CGraphDataItem* CGraphPaneWnd::HitTest(CPoint &pt)
{
	CDC* pDC = m_Offscreen.GetDC();
	if (pDC==NULL) return NULL;
	
	COLORREF clr;
	for(int i=pt.x-1; i<=pt.x+1; i++) {
		for(int j=pt.y-1; j<=pt.y+1; j++) {
			clr=pDC->GetPixel(i,j);
			if ( clr!=RGB(255,255,255)  && clr!=RGB(0,0,0) ) {
				CSeriesArray& Series = m_pData->Series;
				int n = Series.GetSize(),k;
				for(k=0; k<n; k++) {
					if (clr==Series[k]->clr) return Series[k];
				}
			}
		}
	}
	
	return NULL;
	
}

void CGraphPaneWnd::AppendMenuItems(CMenu *menu)
{
    if (menu->GetMenuItemCount() != 0)
    {
		//separator
		menu->AppendMenu(MF_SEPARATOR, 0, _T(""));
    };
    UINT menu_flags = MF_STRING | MF_ENABLED;
	CAxisArray& Axes = m_pData->Axes;
	int n = Axes.GetSize();
	if (n>0) {
		if (Axes[0]->grid)
		{
			menu_flags |= MF_CHECKED;
		}
	}
    menu->AppendMenu(menu_flags, GRAPH_TOGGLE_GRID, _T("Grid"));
    menu->AppendMenu(MF_STRING | MF_ENABLED, GRAPH_FIT, _T("Resize to fit"));
    menu_flags = MF_STRING | MF_ENABLED;
    if (m_bZoomTool)
    {
		menu_flags |= MF_CHECKED;
    };

    menu->AppendMenu(menu_flags, GRAPH_TOGGLE_ZOOM, _T("Zoom Tool"));

}

bool CGraphPaneWnd::MenuCommand(UINT nIDCommand)
{
	if (nIDCommand==GRAPH_TOGGLE_GRID) {
		CAxisArray& Axes = m_pData->Axes;
		int n = Axes.GetSize();
		for(int i=0; i<n; i++) Axes[i]->grid ^= 1;
		return TRUE;
	}
	else if (nIDCommand==GRAPH_FIT) {
		CAxisArray& Axes = m_pData->Axes;
		int n = Axes.GetSize();
		for(int i=0; i<n; i++) Axes[i]->SetAuto();
		return TRUE;
	}
	else if (nIDCommand==GRAPH_TOGGLE_ZOOM) {
		m_bZoomTool = !m_bZoomTool;
		return TRUE;
	}
	else return FALSE;	
}

void CGraphPaneWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//CWnd::OnLButtonDown(nFlags, point);

	if (m_bZoomTool) {
		ptDown = point;

		CClientDC dc(this);
		CRect rect(ptDown.x, ptDown.y, ptDown.x, ptDown.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);
		
		ptLast = ptDown;
		SetCapture();
		m_bZoomStarted = TRUE;
	}
}

void CGraphPaneWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bZoomStarted) {
		CClientDC dc(this);
		CRect zrect(ptDown.x, ptDown.y, ptLast.x, ptLast.y);
		zrect.NormalizeRect();
		dc.DrawFocusRect(zrect);
		ReleaseCapture();
		m_bZoomStarted = FALSE;
		m_bZoomTool = FALSE;

		Zoom(zrect);
	}
}

void CGraphPaneWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//CWnd::OnMouseMove(nFlags, point);
	if (m_bZoomStarted) {
		CClientDC dc(this);
		CRect rect(ptDown.x, ptDown.y, ptLast.x, ptLast.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);
		rect.SetRect(ptDown.x, ptDown.y, point.x, point.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);
		ptLast = point;
	}

	if (m_bZoomTool) SetCursor(AfxGetApp()->LoadCursor(IDC_MAGNIFY));

}

void CGraphPaneWnd::Zoom(CRect &zoom_rect)
{
	CRect client_rect;
	GetClientRect(&client_rect);
	CAxisArray& Axes = m_pData->Axes;
	int n = Axes.GetSize();
	for(int i=0; i<n; i++) Axes[i]->Zoom(client_rect,zoom_rect);
	((CAxisWnd*)GetParent())->Update();
}

BOOL CGraphPaneWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class TTN_NEEDTEXT 
	
	return CWnd::OnNotify(wParam, lParam, pResult);
}
