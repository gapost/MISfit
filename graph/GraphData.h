// GraphData.h: interface for the CGraphData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHDATA_H__B6C48F8E_EC89_4E5E_8392_17130B9E16E6__INCLUDED_)
#define AFX_GRAPHDATA_H__B6C48F8E_EC89_4E5E_8392_17130B9E16E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <float.h>

#define AXIS_BOTTOM 1
#define AXIS_LEFT	2
#define AXIS_TOP	4
#define AXIS_RIGHT	8

#define AXIS_LIN	0
#define AXIS_LOG	1

#define SERIES_LINE 1
#define SERIES_SCATTER 2
#define SERIES_HIST 4

//right button menu commands
enum GRAPH_ITEM_POPUP_COMMANDS
{
    GRAPH_ITEM_POPUP_FIRST	=   0x1000,
	GRAPH_ITEM_PROPS,
	AXIS_TOGGLE_AUTOSCALE,
	GRAPH_FIT,
	GRAPH_TOGGLE_GRID,
	GRAPH_TOGGLE_ZOOM,
    GRAPH_ITEM_POPUP_LAST
};

#define BCKGND_STD RGB(210,210,210)

#define N_MAX_DIV 11

inline
int round(double x)
{
	double f = floor(x);
	if (x-f<0.5)
		return (int)f;
	else
		return ((int)f)+1;
}

class CGraphDataItem
{
public:
	CGraphDataItem() {}
	virtual ~CGraphDataItem() {}
	virtual void AppendMenuItems(CMenu* menu) {}
	virtual bool MenuCommand(UINT nIDCommand);
	virtual void DoPropDlg() {}
};

class CAxis : public CGraphDataItem  
{
public:
	CAxis(int atype, int abase = AXIS_LIN) : 
	  lim(2,0.), 
	  ndiv(0),
	  ticklen(5), 
	  type(atype), 
	  autoscale(true),
	  grid(false),
	  base(abase),
	  size(0),
	  range(0.),
	  nFontHeight(90)
	  {}
	virtual ~CAxis() {}

	void DrawLabels(CDC* pDC, CRect* rect);
	void DrawTicks(CDC* pDC, CRect& rect);
	void DrawTitle(CDC* pDC, CRect* rect);

	void SetTitle(const CString& aTitle) { Title=aTitle; }

	void ResetLimits() { if (autoscale) { lim[0]=0.; lim[1]=0.; range = 0.;} }
	void SetLim(double x0, double x1, bool noauto=false);
	void SetAuto(bool f=true) { autoscale = f; }
	bool IsAuto() { return autoscale; }

	void SetLog(bool f=true) { if (f) base=AXIS_LOG; else base=AXIS_LIN; }

	void SetFontHeight(int cx, int cy);

	int GetBase() { return base; }
	/*int GetScreenOrg() {
		if ((type & AXIS_BOTTOM) || (type & AXIS_TOP)) return 0;
		else return (lim[1]-lim[0])*ax;
	}*/

	int Conv(float x, int& ix);
	int Conv(double x, int& ix);
	int Conv(int x, int& ix);
	int Conv(unsigned int x, int& ix);
	int Conv(unsigned long x, int& ix);

	int Conv(double& w)
	/* convert to 0..1 for horiz and 1..0 for vertical axis
	   x has to be base transformed first!
	*/
	{
		w = ((type & AXIS_BOTTOM) || (type & AXIS_TOP)) ? (w-lim[0])/range : (lim[1]-w)/range;
		return (_finite(w));
	}
	int BaseTransform(double& x)
	/* transform according to axis base */
	{
		x = (base==AXIS_LIN) ? x : log10(x);
		return (_finite(x));
	}
	int InvBaseTransform(double& x)
	/* inverse transform according to axis base */
	{
		x = (base==AXIS_LIN) ? x : pow(10.,x);
		return (_finite(x));
	}

	void Zoom(CRect& old_rect, CRect &new_rect);

	virtual void DoPropDlg();
	virtual bool MenuCommand(UINT nIDCommand);
	virtual void AppendMenuItems(CMenu* menu);
	void FixDiv();

	CArray1D lim;
	int size;
	double div[N_MAX_DIV];
	int ndiv;
	int ticklen;
	int type;
	bool grid;
	CString Title;
protected:
	void PrepareFonts(int nHeight);
	void DrawTick(CDC* pDC, CRect& rect, double val, bool is_sub_tick = false);
	void PrintTickLabel ( char* labelbuf, double val );  
	void DrawLabel(CDC *pDC, CRect* rect, const double& val, const char* labelbuf );

	static CPen GridPen, SubGridPen;

	CFont hfont, vfont;
	int nFontHeight;

	double range;
	int base;
	bool autoscale;
	double tick_spacing;		/* distance between ticks */
	double lin_subtick_spacing;
	int min_tick_count, max_tick_count; /* tick location = count * spacing */
	int lin_subticks;	/* number of linearly spaced subticks */
	bool have_normal_subsubticks; /* does axis have logarithmic subsubticks?*/

	friend class CAxisDlg;

};

class CSeriesBase : public CGraphDataItem
{
public:
	CSeriesBase(CAxis* axaxis, CAxis* ayaxis, 
				COLORREF aclr = 0, int atype = 1) :
	xaxis(axaxis), yaxis(ayaxis), 
	type(atype), clr(aclr)
	{}
	virtual ~CSeriesBase() {}

	// GraphItem functions
	virtual void AppendMenuItems(CMenu* menu);
	virtual void DoPropDlg();

	// Series functions placeholders
	virtual void ScanData() {}
	virtual void Draw(CDC* pDC, CRect& rect) {}
	virtual void SetData(TNT::Array1D<float>& ax, TNT::Array1D<float>& ay) {}
	virtual void SetData(TNT::Array1D<float>& ay) {}
	virtual void SetData(TNT::Array1D<double>& ax, TNT::Array1D<double>& ay) {}
	virtual void SetData(TNT::Array1D<double>& ay) {}
	virtual void SetData(TNT::Array1D<int>& ax, TNT::Array1D<int>& ay) {}
	virtual void SetData(TNT::Array1D<int>& ay) {}
	virtual void SetData(TNT::Array1D<unsigned int>& ax, TNT::Array1D<unsigned int>& ay) {}
	virtual void SetData(TNT::Array1D<unsigned int>& ay) {}
	virtual void SetData(TNT::Array1D<unsigned long>& ax, TNT::Array1D<unsigned long>& ay) {}
	virtual void SetData(TNT::Array1D<unsigned long>& ay) {}
	virtual int IsEmpty() { return 1; }

	int Conv(const CRect& rect, double x, double y, int& i, int& j)
	/* 
	   convert (x,y) first through axis base transformation, then to 0..1,
	   then to screen coords 
	*/
	{
		if (xaxis->BaseTransform(x) && yaxis->BaseTransform(y)
			&& xaxis->Conv(x) && yaxis->Conv(y))
		{
			i = rect.left + round(x*rect.Width());
			j = rect.top + round(y*rect.Height());
			return 1;
		}
		return 0;
	}

	// Attributes
	CAxis *xaxis,*yaxis;
	COLORREF clr;
	int type;
};

template<class T>
class CSeries : public CSeriesBase
{
public:
	CSeries(TNT::Array1D<T>& ax, TNT::Array1D<T>& ay, 
		CAxis* axaxis, CAxis* ayaxis, 
		COLORREF aclr = 0, int atype = 1) : 
	CSeriesBase(axaxis,ayaxis,aclr,atype),
	x(ax), y(ay) 
	{ ScanData(); }
	CSeries(TNT::Array1D<T>& ay, 
		CAxis* axaxis, CAxis* ayaxis, 
		COLORREF aclr = 0, int atype = 1) : 
	CSeriesBase(axaxis,ayaxis,aclr,atype),
	y(ay) 
	{ x = TNT::Array1D<T>(0); ScanData(); }
	virtual ~CSeries() {}

	virtual void ScanData();
	virtual void Draw(CDC* pDC, CRect& rect);
	virtual void SetData(TNT::Array1D<T>& ax, TNT::Array1D<T>& ay)
	{ x = ax; y = ay; ScanData(); }
	virtual void SetData(TNT::Array1D<T>& ay)
	{ x = TNT::Array1D<T>(0); y = ay; ScanData(); }
	virtual int IsEmpty() { return y.dim()==0; }

	TNT::Array1D<T> x,y;
};

typedef CArray<CAxis*,CAxis*> CAxisArray;
typedef CArray<CSeriesBase*,CSeriesBase*> CSeriesArray;

class CGraphData  
{
public:
	CGraphData(int arealtime = 0);
	virtual ~CGraphData();

	void ParseOptionStr(const CString& Opt, int& nopt, COLORREF& clr);
	void setlog(int axisid, bool f = true);
	//void SetSize(int cx, int cy);

	void AddSeries(TNT::Array1D<double>& x, TNT::Array1D<double>& y, const CString& Opt = "")
	{ AddSeries_(new CSeries<double>(x,y,Axes[0],Axes[1]), Opt); }
	void AddSeries(TNT::Array1D<double>& y, const CString& Opt = "")
	{ AddSeries_(new CSeries<double>(y,Axes[0],Axes[1]), Opt); }
	void AddSeries(TNT::Array1D<float>& x, TNT::Array1D<float>& y, const CString& Opt = "")
	{ AddSeries_(new CSeries<float>(x,y,Axes[0],Axes[1]), Opt); }
	void AddSeries(TNT::Array1D<float>& y, const CString& Opt = "")
	{ AddSeries_(new CSeries<float>(y,Axes[0],Axes[1]), Opt); }
	void AddSeries(TNT::Array1D<int>& x, TNT::Array1D<int>& y, const CString& Opt = "")
	{ AddSeries_(new CSeries<int>(x,y,Axes[0],Axes[1]), Opt); }
	void AddSeries(TNT::Array1D<int>& y, const CString& Opt = "")
	{ AddSeries_(new CSeries<int>(y,Axes[0],Axes[1]), Opt); }
	void AddSeries(TNT::Array1D<unsigned int>& x, TNT::Array1D<unsigned int>& y, const CString& Opt = "")
	{ AddSeries_(new CSeries<unsigned int>(x,y,Axes[0],Axes[1]), Opt); }
	void AddSeries(TNT::Array1D<unsigned int>& y, const CString& Opt = "")
	{ AddSeries_(new CSeries<unsigned int>(y,Axes[0],Axes[1]), Opt); }
	void AddSeries(TNT::Array1D<unsigned long>& x, TNT::Array1D<unsigned long>& y, const CString& Opt = "")
	{ AddSeries_(new CSeries<unsigned long>(x,y,Axes[0],Axes[1]), Opt); }
	void AddSeries(TNT::Array1D<unsigned long>& y, const CString& Opt = "")
	{ AddSeries_(new CSeries<unsigned long>(y,Axes[0],Axes[1]), Opt); }

	void XLabel(const CString& S) { Axes[0]->SetTitle(S); }
	void YLabel(const CString& S) { Axes[1]->SetTitle(S); }
	void XLim(double x0, double x1) 
	{ 
		CAxis* p = Axes[0];
		if (p->BaseTransform(x0) && p->BaseTransform(x1))
			p->SetLim(x0,x1,true); 
	}
	void YLim(double x0, double x1)
	{ 
		CAxis* p = Axes[1];
		if (p->BaseTransform(x0) && p->BaseTransform(x1))
			p->SetLim(x0,x1,true); 
	}
	void Destroy();
	void Create();
	
	int defclr;
	CAxisArray Axes;
	CSeriesArray Series;
protected:
	void AddSeries_(CSeriesBase* pSeries,const CString& Opt);
	int realtime;
};

#endif // !defined(AFX_GRAPHDATA_H__B6C48F8E_EC89_4E5E_8392_17130B9E16E6__INCLUDED_)
