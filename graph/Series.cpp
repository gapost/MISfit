// GraphData.cpp: implementation of the CGraphData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "GraphData.h"
#include "GraphDlg.h"

#include <float.h>

class MinMaxFinder
{
public:
	double Min,Max;
	int start;
	MinMaxFinder() : start(1) {}
	void operator<<(const double& v)
	{
		if (start) 
		{
			Min = v;
			Max = v;
			start = 0;
		}
		else
		{
			if (v>Max) Max = v;
			else if (v<Min) Min = v;
		}
	}
	bool HasValidData() { return start==0; }
};

void CSeriesBase::DoPropDlg()
{
	CSeriesDlg dlg;
	dlg.SetType(type);
	if (dlg.DoModal()==IDOK) type = dlg.GetType();
}

void CSeriesBase::AppendMenuItems(CMenu *menu)
{
    if (menu->GetMenuItemCount() != 0)
    {
		//separator
		menu->AppendMenu(MF_SEPARATOR, 0, _T(""));
    };
    menu->AppendMenu(MF_STRING | MF_ENABLED, GRAPH_ITEM_PROPS, _T("Data Properties"));
}

template class CSeries<float>;
template class CSeries<double>;
template class CSeries<int>;
template class CSeries<unsigned int>;
template class CSeries<unsigned long>;

template<class T>
void CSeries<T>::ScanData()
{
	if (!xaxis->IsAuto() && !yaxis->IsAuto()) return;
	
	ASSERT(y.dim()>0);

	bool has_x = x.dim()>0;
	int n = y.dim();
	MinMaxFinder XLim,YLim;

	for(int i=0; i<n; i++)
	{
		double ax = has_x ? x[i] : i+1;
		double ay = y[i];
		if (xaxis->BaseTransform(ax) && yaxis->BaseTransform(ay))
		{
			XLim << ax;
			YLim << ay;
		}
	}
	if (XLim.HasValidData() && xaxis->IsAuto()) xaxis->SetLim(XLim.Min,XLim.Max);
	if (YLim.HasValidData() && yaxis->IsAuto()) yaxis->SetLim(YLim.Min,YLim.Max);
}

template<class T>
void CSeries<T>::Draw(CDC *pDC, CRect& rect)
{
	int n = y.dim();
	if (n==0) return;
	
	CPen pen(PS_SOLID, 1, clr);
	CBrush brush(clr);
	
	CGdiObject *pOldPen = pDC->SelectObject(&pen);
	CGdiObject *pOldBrush = pDC->SelectObject(&brush);
	
	bool have_x = x.dim()>0;
	int i,j,k;

	if (type & SERIES_HIST)
	{
		for(k=0; k<n; k++)
		{
			double vx = have_x ? x[k] : k+1;
			if (Conv(rect,vx,y[k],i,j))
			{
				pDC->PatBlt(i-1,j,3,rect.bottom-j,PATCOPY);
			}
		}
	}
	else
	{
		int bline = type & SERIES_LINE, bscat = type & SERIES_SCATTER;
		bool linecont = false;

		for(k=0; k<n; k++) 
		{
			double vx = have_x ? x[k] : k+1;
			if (Conv(rect,vx,y[k],i,j))
			{
				if (bline) 
				{
					if (linecont) pDC->LineTo(i,j);
					else 
					{
						pDC->MoveTo(i,j);
						linecont = true;
					}
				}
				if (bscat) pDC->PatBlt(i-1,j-1,3,3,PATCOPY);
			}
			else linecont=false;
		}

	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

