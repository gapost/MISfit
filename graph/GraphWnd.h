#if !defined(AFX_GRAPHWND_H__C67A2CA4_BADB_4DC8_B283_5D6506AF067B__INCLUDED_)
#define AFX_GRAPHWND_H__C67A2CA4_BADB_4DC8_B283_5D6506AF067B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraphWnd window

class CGraphData;
class CAxisWnd;

class AFX_EXT_CLASS CGraphWnd : public CWnd
{
DECLARE_DYNCREATE(CGraphWnd)
// Construction
public:
	CGraphWnd();

// Attributes
public:
	CAxisWnd* m_pAxisWnd;
	CGraphData* m_pData;
	int nRow,nCol,nPlot,nActive;
	CRect* rPane;

// Operations
public:
	BOOL Create(const RECT& rect, CWnd* pParentWnd, int aRow = 1, int aCol = 1);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphWnd)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:

	// double plot routines
	void plot(TNT::Array1D<double>& y, const CString& Opt = "");
	void plot(TNT::Array2D<double>& y, const CString& Opt = "");
	void plot(TNT::Array1D<double>& x, TNT::Array1D<double>& y, const CString& Opt = "");
	void plot(TNT::Array1D<double>& x, TNT::Array2D<double>& y, const CString& Opt = "");
	void update(TNT::Array1D<double>& y, int i = 0);
	void update(TNT::Array1D<double>& x, TNT::Array1D<double>& y, int i = 0);
	void update(TNT::Array2D<double>& y);
	void update(TNT::Array1D<double>& x, TNT::Array2D<double>& y);

	// float plot routines
	void plot(TNT::Array1D<float>& y, const CString& Opt = "");
	void plot(TNT::Array2D<float>& y, const CString& Opt = "");
	void plot(TNT::Array1D<float>& x, TNT::Array1D<float>& y, const CString& Opt = "");
	void plot(TNT::Array1D<float>& x, TNT::Array2D<float>& y, const CString& Opt = "");
	void update(TNT::Array1D<float>& y, int i = 0);
	void update(TNT::Array1D<float>& x, TNT::Array1D<float>& y, int i = 0);
	void update(TNT::Array2D<float>& y);
	void update(TNT::Array1D<float>& x, TNT::Array2D<float>& y);

	// int plot routines
	void plot(TNT::Array1D<int>& y, const CString& Opt = "");
	void plot(TNT::Array2D<int>& y, const CString& Opt = "");
	void plot(TNT::Array1D<int>& x, TNT::Array1D<int>& y, const CString& Opt = "");
	void plot(TNT::Array1D<int>& x, TNT::Array2D<int>& y, const CString& Opt = "");
	void update(TNT::Array1D<int>& y, int i = 0);
	void update(TNT::Array1D<int>& x, TNT::Array1D<int>& y, int i = 0);
	void update(TNT::Array2D<int>& y);
	void update(TNT::Array1D<int>& x, TNT::Array2D<int>& y);

	// unsigned int plot routines
	void plot(TNT::Array1D<unsigned int>& y, const CString& Opt = "");
	void plot(TNT::Array2D<unsigned int>& y, const CString& Opt = "");
	void plot(TNT::Array1D<unsigned int>& x, TNT::Array1D<unsigned int>& y, const CString& Opt = "");
	void plot(TNT::Array1D<unsigned int>& x, TNT::Array2D<unsigned int>& y, const CString& Opt = "");
	void update(TNT::Array1D<unsigned int>& y, int i = 0);
	void update(TNT::Array1D<unsigned int>& x, TNT::Array1D<unsigned int>& y, int i = 0);
	void update(TNT::Array2D<unsigned int>& y);
	void update(TNT::Array1D<unsigned int>& x, TNT::Array2D<unsigned int>& y);

	// unsigned long plot routines
	void plot(TNT::Array1D<unsigned long>& y, const CString& Opt = "");
	void plot(TNT::Array2D<unsigned long>& y, const CString& Opt = "");
	void plot(TNT::Array1D<unsigned long>& x, TNT::Array1D<unsigned long>& y, const CString& Opt = "");
	void plot(TNT::Array1D<unsigned long>& x, TNT::Array2D<unsigned long>& y, const CString& Opt = "");
	void update(TNT::Array1D<unsigned long>& y, int i = 0);
	void update(TNT::Array1D<unsigned long>& x, TNT::Array1D<unsigned long>& y, int i = 0);
	void update(TNT::Array2D<unsigned long>& y);
	void update(TNT::Array1D<unsigned long>& x, TNT::Array2D<unsigned long>& y);

	// plot options
	void logx(bool f = true);
	void logy(bool f = true);
	void grid(bool on = true);
	void subplot(int iActive);

	// annotations
	void xlabel(const CString& S);
	void ylabel(const CString& S);
	void xlim(double x0, double x1);
	void ylim(double x0, double x1);

	// operations
	void UpdatePlot();
	void Clear();
	void SetBackClr(COLORREF clr);

	void Layout(int cx, int cy);
	virtual ~CGraphWnd();
	//afx_msg void OnEditCopyWmf(); 

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHWND_H__C67A2CA4_BADB_4DC8_B283_5D6506AF067B__INCLUDED_)
