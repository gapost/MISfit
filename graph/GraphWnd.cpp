// GraphWnd.cpp : implementation file
//

#include "stdafx.h"
#include "GraphWnd.h"

#include "AxisWnd.h"
#include "GraphPaneWnd.h"
#include "GraphData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphWnd

IMPLEMENT_DYNCREATE(CGraphWnd, CWnd)

CGraphWnd::CGraphWnd()
{
	m_pAxisWnd = NULL;
	m_pData = NULL;
	rPane = NULL;
}

CGraphWnd::~CGraphWnd()
{
	if (m_pAxisWnd) delete [] m_pAxisWnd;
	if (m_pData) delete [] m_pData;
	if (rPane) delete [] rPane;
}


BEGIN_MESSAGE_MAP(CGraphWnd, CWnd)
	//{{AFX_MSG_MAP(CGraphWnd)
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_PARENTNOTIFY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGraphWnd message handlers

BOOL CGraphWnd::Create(const RECT& rect, CWnd* pParentWnd, int aRow, int aCol) 
{
	nRow = aRow;
	nCol = aCol;
	nPlot = nRow*nCol;
	if (nPlot<=0) {
		nPlot = 1;
		nCol = 1;
		nRow = 1;
	}
	nActive = 0;
	
	BOOL ret = CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParentWnd, 10);

	if (!ret) return ret;

	if (m_pAxisWnd) delete [] m_pAxisWnd;
	if (m_pData) delete [] m_pData;
	if (rPane) delete [] rPane;

	m_pAxisWnd = new CAxisWnd[nPlot];
	m_pData = new CGraphData[nPlot];
	rPane = new CRect[nPlot];

	CRect crect(rect);
	Layout(crect.Width(),crect.Height());

	for(int i=0; i<nPlot; i++) {
		ret = m_pAxisWnd[i].Create(m_pData+i, rPane[i], this, 10+i);
		if (!ret) return ret;
	}

	return ret;
}

void CGraphWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (m_pAxisWnd) {
		Layout(cx,cy);
		for(int i=0; i<nPlot; i++) 
			if (::IsWindow(m_pAxisWnd[i].m_hWnd) ) m_pAxisWnd[i].MoveWindow(rPane+i,FALSE);
	}
}

void CGraphWnd::OnParentNotify(UINT message, LPARAM lParam) 
{
	CWnd::OnParentNotify(message, lParam);

	if (LOWORD(message)==WM_RBUTTONDOWN) {
			//CString S;
			CPoint pt(GetMessagePos());
			ScreenToClient(&pt);
			for(int i=0; i<nPlot; i++) {
				if (rPane[i].PtInRect(pt)) {
					pt.x -= rPane[i].left;
					pt.y -= rPane[i].top;
					m_pAxisWnd[i].PopupMenu(pt);
					return;
				}
			}
	}
}

/*********************************************
	plot routines
**********************************************/
// double
void CGraphWnd::plot(TNT::Array1D<double>& y, const CString& Opt)
{ 	m_pData[nActive].AddSeries(y,Opt); m_pAxisWnd[nActive].Update(); }
void CGraphWnd::plot(TNT::Array2D<double>& y, const CString& Opt)
{ 		
	int M = y.dim1(), N = y.dim2();
	for (int i = 0; i<M; i++) m_pData[nActive].AddSeries(CArray1D(N,y[i]),Opt);
	m_pAxisWnd[nActive].Update(); 
}
void CGraphWnd::plot(TNT::Array1D<double>& x, TNT::Array1D<double>& y, const CString& Opt)
{ 	m_pData[nActive].AddSeries(x,y,Opt); m_pAxisWnd[nActive].Update(); }
void CGraphWnd::plot(TNT::Array1D<double>& x, TNT::Array2D<double>& y, const CString& Opt)
{ 		
	int M = y.dim1(), N = y.dim2();
	for (int i = 0; i<M; i++) m_pData[nActive].AddSeries(x,TNT::Array1D<double>(N,y[i]),Opt);
	m_pAxisWnd[nActive].Update(); 
}
void CGraphWnd::update(TNT::Array1D<double>& y, int i)
{
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (i<n) {
		S[i]->SetData(y);
		m_pAxisWnd[nActive].Update();
	}
}
void CGraphWnd::update(TNT::Array1D<double>& x, TNT::Array1D<double>& y, int i)
{
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (i<n) {
		S[i]->SetData(x,y);
		m_pAxisWnd[nActive].Update();
	}
}
void CGraphWnd::update(TNT::Array2D<double>& y)
{
	int M = y.dim1(), N = y.dim2();
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (n==M) {
		for (int i = 0; i<M; i++) S[i]->SetData(TNT::Array1D<double>(N,y[i]));
		m_pAxisWnd[nActive].Update();
	}
}
void CGraphWnd::update(TNT::Array1D<double>& x, TNT::Array2D<double>& y)
{
	int M = y.dim1(), N = y.dim2();
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (n==M) {
		for (int i = 0; i<M; i++) S[i]->SetData(x,TNT::Array1D<double>(N,y[i]));
		m_pAxisWnd[nActive].Update();
	}
}
// float
void CGraphWnd::plot(TNT::Array1D<float>& y, const CString& Opt)
{ 	m_pData[nActive].AddSeries(y,Opt); m_pAxisWnd[nActive].Update(); }
void CGraphWnd::plot(TNT::Array2D<float>& y, const CString& Opt)
{ 		
	int M = y.dim1(), N = y.dim2();
	for (int i = 0; i<M; i++) m_pData[nActive].AddSeries(TNT::Array1D<float>(N,y[i]),Opt);
	m_pAxisWnd[nActive].Update(); 
}
void CGraphWnd::plot(TNT::Array1D<float>& x, TNT::Array1D<float>& y, const CString& Opt)
{ 	m_pData[nActive].AddSeries(x,y,Opt); m_pAxisWnd[nActive].Update(); }
void CGraphWnd::plot(TNT::Array1D<float>& x, TNT::Array2D<float>& y, const CString& Opt)
{ 		
	int M = y.dim1(), N = y.dim2();
	for (int i = 0; i<M; i++) m_pData[nActive].AddSeries(x,TNT::Array1D<float>(N,y[i]),Opt);
	m_pAxisWnd[nActive].Update(); 
}
void CGraphWnd::update(TNT::Array1D<float>& y, int i)
{
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (i<n) {
		S[i]->SetData(y);
		m_pAxisWnd[nActive].Update();
	}
}
void CGraphWnd::update(TNT::Array1D<float>& x, TNT::Array1D<float>& y, int i)
{
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (i<n) {
		S[i]->SetData(x,y);
		m_pAxisWnd[nActive].Update();
	}
}
void CGraphWnd::update(TNT::Array2D<float>& y)
{
	int M = y.dim1(), N = y.dim2();
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (n==M) {
		for (int i = 0; i<M; i++) S[i]->SetData(TNT::Array1D<float>(N,y[i]));
		m_pAxisWnd[nActive].Update();
	}
}
void CGraphWnd::update(TNT::Array1D<float>& x, TNT::Array2D<float>& y)
{
	int M = y.dim1(), N = y.dim2();
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (n==M) {
		for (int i = 0; i<M; i++) S[i]->SetData(x,TNT::Array1D<float>(N,y[i]));
		m_pAxisWnd[nActive].Update();
	}
}
// int
void CGraphWnd::plot(TNT::Array1D<int>& y, const CString& Opt)
{ 	m_pData[nActive].AddSeries(y,Opt); m_pAxisWnd[nActive].Update(); }
void CGraphWnd::plot(TNT::Array2D<int>& y, const CString& Opt)
{ 		
	int M = y.dim1(), N = y.dim2();
	for (int i = 0; i<M; i++) m_pData[nActive].AddSeries(TNT::Array1D<int>(N,y[i]),Opt);
	m_pAxisWnd[nActive].Update(); 
}
void CGraphWnd::plot(TNT::Array1D<int>& x, TNT::Array1D<int>& y, const CString& Opt)
{ 	m_pData[nActive].AddSeries(x,y,Opt); m_pAxisWnd[nActive].Update(); }
void CGraphWnd::plot(TNT::Array1D<int>& x, TNT::Array2D<int>& y, const CString& Opt)
{ 		
	int M = y.dim1(), N = y.dim2();
	for (int i = 0; i<M; i++) m_pData[nActive].AddSeries(x,TNT::Array1D<int>(N,y[i]),Opt);
	m_pAxisWnd[nActive].Update(); 
}
void CGraphWnd::update(TNT::Array1D<int>& y, int i)
{
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (i<n) {
		S[i]->SetData(y);
		m_pAxisWnd[nActive].Update();
	}
}
void CGraphWnd::update(TNT::Array1D<int>& x, TNT::Array1D<int>& y, int i)
{
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (i<n) {
		S[i]->SetData(x,y);
		m_pAxisWnd[nActive].Update();
	}
}
void CGraphWnd::update(TNT::Array2D<int>& y)
{
	int M = y.dim1(), N = y.dim2();
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (n==M) {
		for (int i = 0; i<M; i++) S[i]->SetData(TNT::Array1D<int>(N,y[i]));
		m_pAxisWnd[nActive].Update();
	}
}
void CGraphWnd::update(TNT::Array1D<int>& x, TNT::Array2D<int>& y)
{
	int M = y.dim1(), N = y.dim2();
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (n==M) {
		for (int i = 0; i<M; i++) S[i]->SetData(x,TNT::Array1D<int>(N,y[i]));
		m_pAxisWnd[nActive].Update();
	}
}
// unsigned int
void CGraphWnd::plot(TNT::Array1D<unsigned int>& y, const CString& Opt)
{ 	m_pData[nActive].AddSeries(y,Opt); m_pAxisWnd[nActive].Update(); }
void CGraphWnd::plot(TNT::Array2D<unsigned int>& y, const CString& Opt)
{ 		
	int M = y.dim1(), N = y.dim2();
	for (int i = 0; i<M; i++) m_pData[nActive].AddSeries(TNT::Array1D<unsigned int>(N,y[i]),Opt);
	m_pAxisWnd[nActive].Update(); 
}
void CGraphWnd::plot(TNT::Array1D<unsigned int>& x, TNT::Array1D<unsigned int>& y, const CString& Opt)
{ 	m_pData[nActive].AddSeries(x,y,Opt); m_pAxisWnd[nActive].Update(); }
void CGraphWnd::plot(TNT::Array1D<unsigned int>& x, TNT::Array2D<unsigned int>& y, const CString& Opt)
{ 		
	int M = y.dim1(), N = y.dim2();
	for (int i = 0; i<M; i++) m_pData[nActive].AddSeries(x,TNT::Array1D<unsigned int>(N,y[i]),Opt);
	m_pAxisWnd[nActive].Update(); 
}
void CGraphWnd::update(TNT::Array1D<unsigned int>& y, int i)
{
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (i<n) {
		S[i]->SetData(y);
		m_pAxisWnd[nActive].Update();
	}
}
void CGraphWnd::update(TNT::Array1D<unsigned int>& x, TNT::Array1D<unsigned int>& y, int i)
{
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (i<n) {
		S[i]->SetData(x,y);
		m_pAxisWnd[nActive].Update();
	}
}
void CGraphWnd::update(TNT::Array2D<unsigned int>& y)
{
	int M = y.dim1(), N = y.dim2();
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (n==M) {
		for (int i = 0; i<M; i++) S[i]->SetData(TNT::Array1D<unsigned int>(N,y[i]));
		m_pAxisWnd[nActive].Update();
	}
}
void CGraphWnd::update(TNT::Array1D<unsigned int>& x, TNT::Array2D<unsigned int>& y)
{
	int M = y.dim1(), N = y.dim2();
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (n==M) {
		for (int i = 0; i<M; i++) S[i]->SetData(x,TNT::Array1D<unsigned int>(N,y[i]));
		m_pAxisWnd[nActive].Update();
	}
}
// unsigned long
void CGraphWnd::plot(TNT::Array1D<unsigned long>& y, const CString& Opt)
{ 	m_pData[nActive].AddSeries(y,Opt); m_pAxisWnd[nActive].Update(); }
void CGraphWnd::plot(TNT::Array2D<unsigned long>& y, const CString& Opt)
{ 		
	int M = y.dim1(), N = y.dim2();
	for (int i = 0; i<M; i++) m_pData[nActive].AddSeries(TNT::Array1D<unsigned long>(N,y[i]),Opt);
	m_pAxisWnd[nActive].Update(); 
}
void CGraphWnd::plot(TNT::Array1D<unsigned long>& x, TNT::Array1D<unsigned long>& y, const CString& Opt)
{ 	m_pData[nActive].AddSeries(x,y,Opt); m_pAxisWnd[nActive].Update(); }
void CGraphWnd::plot(TNT::Array1D<unsigned long>& x, TNT::Array2D<unsigned long>& y, const CString& Opt)
{ 		
	int M = y.dim1(), N = y.dim2();
	for (int i = 0; i<M; i++) m_pData[nActive].AddSeries(x,TNT::Array1D<unsigned long>(N,y[i]),Opt);
	m_pAxisWnd[nActive].Update(); 
}
void CGraphWnd::update(TNT::Array1D<unsigned long>& y, int i)
{
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (i<n) {
		S[i]->SetData(y);
		m_pAxisWnd[nActive].Update();
	}
}
void CGraphWnd::update(TNT::Array1D<unsigned long>& x, TNT::Array1D<unsigned long>& y, int i)
{
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (i<n) {
		S[i]->SetData(x,y);
		m_pAxisWnd[nActive].Update();
	}
}
void CGraphWnd::update(TNT::Array2D<unsigned long>& y)
{
	int M = y.dim1(), N = y.dim2();
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (n==M) {
		for (int i = 0; i<M; i++) S[i]->SetData(TNT::Array1D<unsigned long>(N,y[i]));
		m_pAxisWnd[nActive].Update();
	}
}
void CGraphWnd::update(TNT::Array1D<unsigned long>& x, TNT::Array2D<unsigned long>& y)
{
	int M = y.dim1(), N = y.dim2();
	CSeriesArray& S = m_pData[nActive].Series;
	int n = S.GetSize();
	if (n==M) {
		for (int i = 0; i<M; i++) S[i]->SetData(x,TNT::Array1D<unsigned long>(N,y[i]));
		m_pAxisWnd[nActive].Update();
	}
}
/*********************************************
	end plot routines
**********************************************/


void CGraphWnd::xlabel(const CString& S) 
{ 
	m_pData[nActive].XLabel(S); 
	UpdatePlot(); 
}
void CGraphWnd::ylabel(const CString& S) 
{ 
	m_pData[nActive].YLabel(S); 
	UpdatePlot(); 
}

void CGraphWnd::xlim(double x0, double x1) 
{ 
	m_pData[nActive].XLim(x0,x1); 
	UpdatePlot(); 
}


void CGraphWnd::ylim(double x0, double x1) 
{ 
	m_pData[nActive].YLim(x0,x1); 
	UpdatePlot(); 
}

void CGraphWnd::grid(bool on)
{
	CAxisArray& Axes = m_pData[nActive].Axes;
	int n = Axes.GetSize();
	for(int i=0; i<n; i++) Axes[i]->grid = on;
	UpdatePlot();
}

void CGraphWnd::UpdatePlot() 
{ 
	m_pAxisWnd[nActive].Update(); 
}
void CGraphWnd::Clear() 
{ 
	m_pData[nActive].Create(); 
}
void CGraphWnd::SetBackClr(COLORREF clr)
{
	for(int i=0; i<nPlot; i++) m_pAxisWnd[i].backclr = clr;
	UpdatePlot();
}

void CGraphWnd::Layout(int cx, int cy)
{
	int k = 0;
	int x1,y1,x2,y2;
	int w = cx/nCol, h=cy/nRow;
	for(int i=0; i<nRow; i++) {
		for(int j=0; j<nCol; j++) {
			x1 = j*w;
			y1 = i*h;
			x2 = (j+1)*w;
			y2 = (i+1)*h;
			if (j+1==nCol) x2 = cx;
			if (i+1==nRow) y2 = cy;
			rPane[k].SetRect(x1,y1,x2,y2);
			k++;
		}
	}

}

void CGraphWnd::subplot(int iActive)
{
	iActive--;
	if (iActive<nPlot && iActive>=0) nActive = iActive;

}

/*
void CGraphWnd::OnEditCopyWmf() 
{ 
	tagSTGMEDIUM * data = new tagSTGMEDIUM;
	CMetaFileDC * dcMeta = new CMetaFileDC();
	COleDataSource* pData = new COleDataSource;

	CClientDC dcRef( this );
	CRect lmbr;
	GetClientRect(lmbr);

	//replace "ImageName" with a description of this image
	BOOL ret = dcMeta->CreateEnhanced(&dcRef, NULL, &lmbr,"ImageName\0Copy\0\0" );
	dcMeta->SetMapMode(MM_TEXT);

	//actually draw to the device context
	//OnDraw(dcMeta);
	for(int i=0; i<nPlot; i++) {
		m_pAxisWnd[i].DrawHere(dcMeta,rPane[i]);
	}

	//done drawing
	data->tymed = TYMED_ENHMF;
	data->hEnhMetaFile = dcMeta->CloseEnhanced();
	pData->CacheData( CF_ENHMETAFILE, data );
	pData->SetClipboard();
	//delete dcMeta;
	//delete data;
}
*/

void CGraphWnd::logx(bool f)
{
	m_pData[nActive].setlog(0,f); 
	UpdatePlot(); 

}

void CGraphWnd::logy(bool f)
{
	m_pData[nActive].setlog(1,f); 
	UpdatePlot(); 
}
