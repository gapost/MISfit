// GraphData.cpp: implementation of the CGraphData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <float.h>
#include "GraphData.h"

#define N_COLORS 7

static COLORREF SERIES_COLOR[] = {
	RGB(0,0,255), // blue
	RGB(255,0,0), // red
	RGB(0,255,0), // green
	RGB(190,0,190), // magenta
	RGB(0,190,190), // see blue
	RGB(190,190,0), // olive
	RGB(0,0,1) // black
};

static char COLOR_CODES[] = { 'b','r','g','m','s','o','k' };

int findcolor(char c)
{
	for(int i=0; i<N_COLORS; i++) {
		if (c==COLOR_CODES[i]) return i;
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraphData::CGraphData(int arealtime) 
{ 
	realtime = arealtime;
	Axes.SetSize(2,2);
	Axes.SetAt(0, new CAxis(AXIS_BOTTOM | AXIS_TOP) );
	Axes.SetAt(1, new CAxis(AXIS_LEFT | AXIS_RIGHT) );
	Create(); 
}
CGraphData::~CGraphData() 
{
	int i,n=Axes.GetSize();
	if (n>0) {
		for(i=0; i<n; i++) delete Axes[i];
		Axes.RemoveAll();
	}
	n=Series.GetSize();
	if (n>0) {
		for(i=0; i<n; i++) delete Series[i];
		Series.RemoveAll();
	}
}
void CGraphData::Destroy()
{
	int n=Series.GetSize(),i;
	if (n>0) {
		for(i=0; i<n; i++) delete Series[i];
		Series.RemoveAll();
	}
	Series.SetSize(0,5);
}

void CGraphData::Create()
{
	Destroy();
	defclr = 0;
}

void CGraphData::ParseOptionStr(const CString &Opt, int &nopt, COLORREF &clr)
{
	int nclr = -1;
	nopt = 0;
	int nchar = Opt.GetLength();
	for(int i=0; i<nchar; i++) {
		if (Opt[i]=='.') nopt = nopt | SERIES_SCATTER;
		else if (Opt[i]=='-') nopt = nopt | SERIES_LINE;
		else if (Opt[i]=='h') nopt = nopt | SERIES_HIST;
		else nclr=findcolor(Opt[i]);
	}
	if (nclr<0) nclr = (defclr++) % N_COLORS;
	if (nopt==0) nopt = SERIES_LINE;

	clr = SERIES_COLOR[nclr];
}

void CGraphData::AddSeries_(CSeriesBase* pSeries, const CString& Opt)
{
	// parse options
	int nopt;
	COLORREF clr;
	ParseOptionStr(Opt,nopt,clr);

	pSeries->clr = clr;
	pSeries->type = nopt;

	int n = Series.GetSize();
	Series.SetAtGrow( n, pSeries );

	n = Axes.GetSize();
	for(int i = 0; i<n; i++) Axes[i]->FixDiv();
}

/*void CGraphData::SetSize(int cx, int cy)
{
	int naxes = Axes.GetSize();
	for(int i=0; i<naxes; i++) {
		int axtype = Axes[i]->type;
		if ((axtype & AXIS_BOTTOM) || (axtype & AXIS_TOP)) Axes[i]->SetSize(cx);
		else Axes[i]->SetSize(cy);
	}
}*/

void CGraphData::setlog(int axisid, bool f)
{
	int naxes = Axes.GetSize();
	for(int i=0; i<naxes; i++) {
		int axtype = Axes[i]->type;
		if (((axtype & AXIS_BOTTOM) || (axtype & AXIS_TOP))) {
			if (axisid==0) Axes[i]->SetLog(f);
		}
		else {
			if (axisid==1) Axes[i]->SetLog(f);
		}
	}
}


bool CGraphDataItem::MenuCommand(UINT nIDCommand) 
{
	if (nIDCommand==GRAPH_ITEM_PROPS) {
		DoPropDlg();
		return true;
	}
	else return false;
}









