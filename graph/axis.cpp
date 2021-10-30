// axis.cpp: implementation of the CAxis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <float.h>
#include "GraphData.h"
#include "GraphDlg.h"

#define FUZZ 0.000001		/* bd. on floating pt. roundoff error */

#define NEAR_EQUALITY(a, b, scale) (fabs((a) - (b)) < (FUZZ * fabs(scale)))


#define TRIAL_NUMBER_OF_TICK_INTERVALS 5
#define MAX_NUM_SUBTICKS 29	/* max num. of linearly spaced subticks */
#define RELATIVE_SUBTICK_SIZE 0.5 /* subtick_size / tick_size */
/* if a log axis spans >5.0 orders of magnitude, don't plot log subsubticks */
#define MAX_DECADES_WITH_LOG_SUBSUBTICKS 4.0 

/* inter-tick spacing types, returned by scale1() and spacing_type() */
#define S_ONE 0
#define S_TWO 1
#define S_FIVE 2

CPen CAxis::GridPen(PS_DOT,1,RGB(0,0,0));
CPen CAxis::SubGridPen(PS_DOT,1,RGB(192,192,192));

/* Axis drawing functions */

void CAxis::PrintTickLabel ( char* labelbuf, double val )  
{
	int prec;
	char *eloc, *ptr;
	char labelbuf_tmp[64], incrbuf[64];
	bool big_exponents;
	double minv, maxv;
	
	/* two possibilities: large/small exponent magnitudes */
	
	minv = (base == AXIS_LOG 
		? pow (10.0, lim[0]) : lim[1]);
	maxv = (base == AXIS_LOG 
		? pow (10.0, lim[1]) : lim[1]);
	val = (base == AXIS_LOG 
		? pow (10.0, val) : val);
	
	big_exponents = (((minv != 0.0 && fabs (log10 (fabs (minv))) >= 4.0)
		|| (maxv != 0.0 && fabs (log10 (fabs (maxv))) >= 4.0))
		? true : false);
	
	if (big_exponents)
	/* large exponents, rewrite as foo x 10^bar, using escape sequences */
    {
		char *src = labelbuf_tmp, *dst = labelbuf;
		int exponent;
		char floatbuf[64];
		char *fptr = floatbuf;
		double prefactor;
		
		sprintf (labelbuf_tmp, "%e", val);
		if ((eloc = strchr (labelbuf_tmp, (int)'e')) == NULL)
			return;
		
		if (base == AXIS_LOG) // && !axis->user_specified_subsubticks)
		/* a hack: this must be a power of 10, so just print "10^bar" */
		{
			sscanf (++eloc, "%d", &exponent);	      
			sprintf (dst, "1e%d", exponent);
			return;
		}
		
		/* special case: zero prints as `0', not 0.0x10^whatever */
		if (val == 0.0)
		{
			*dst++ = '0';
			*dst = '\0';
			return;
		}
		
		while (src < eloc)
			*fptr++ = *src++;
		*fptr = '\0';
		sscanf (floatbuf, "%lf", &prefactor); /* get foo */
		sscanf (++src, "%d", &exponent); /* get bar */
		
		sprintf (incrbuf, "%f", 
			tick_spacing / pow (10.0, (double)exponent));
		ptr = strchr (incrbuf, (int)'.');
		prec = 0;
		if (ptr != NULL)
		{
			int count = 0;
			
			while (*(++ptr))
			{
				count++;
				if (*ptr != '0')
					prec = count;
			}
		}
		
		/* \sp ... \ep is start_superscript ... end_superscript, and \r6 is
		right-shift by 1/6 em.  \mu is the `times' character. */
		sprintf (dst, "%.*fe%d", 
			prec, prefactor, exponent);
		
		return;
    }
	
	else	/* small-size exponent magnitudes */
    {
		if (base == AXIS_LOG) // && !axis->user_specified_subsubticks)
		/* a hack: this must be a (small) power of 10, so we'll just use
		%g format (same as %f, no trailing zeroes) */
		{
			sprintf (labelbuf, "%.9g", val);
			return;
		}
		
		sprintf (incrbuf, "%.9f", tick_spacing);
		ptr = strchr (incrbuf, (int)'.');
		prec = 0;
		if (ptr != NULL)
		{
			int count = 0;
			
			while (*(++ptr))
			{
				count++;
				if (*ptr != '0')
					prec = count;
			}
		}
		sprintf (labelbuf, "%.*f", prec, val);
		return;
    }
}

void CAxis::DrawTick(CDC* pDC, CRect& rect, double w, bool is_sub_tick)
// Draw a tick and a grid line
// val is (x-x0)/(x1-x0)
{
	CGdiObject *pOldPen = pDC->SelectStockObject(BLACK_PEN);
	CPen* pGridPen = is_sub_tick ? &(CAxis::SubGridPen) : &(CAxis::GridPen);
	int len = is_sub_tick ? round(RELATIVE_SUBTICK_SIZE*ticklen) : ticklen;

	int cx = rect.Width(), cy = rect.Height();
	int x0 = rect.left, y0 = rect.top;
	int i,j;

	if (type & AXIS_BOTTOM)
	{
		i = round(w*cx);
		if (i>0 && i<cx)
		{
			i += x0;
			j = y0 + cy;
			if (grid) {
				CGdiObject* pOldPen2 = pDC->SelectObject(pGridPen);
				pDC->MoveTo(i, j);
				pDC->LineTo(i, j - cy);
				pDC->SelectObject(pOldPen2);
			}
			pDC->MoveTo( i, j);
			pDC->LineTo( i, j - len);
		}
	}
	if (type & AXIS_TOP)
	{
		i = round(w*cx);
		if (i>0 && i<cx)
		{
			i += x0;
			j = y0;
			pDC->MoveTo(i,j);
			pDC->LineTo(i,j + len);
		}
	}
	if (type & AXIS_LEFT)
	{
		j = round(w*cy);
		if (j>0 && j<cy)
		{
			i = x0;
			j += y0;
			if (grid)
			{
				CGdiObject* pOldPen2 = pDC->SelectObject(pGridPen);
				pDC->MoveTo(i,j);
				pDC->LineTo(i+cx,j);
				pDC->SelectObject(pOldPen2);
			}
			pDC->MoveTo(i,j);
			pDC->LineTo(i+len,j);
		}
	}
	if (type & AXIS_RIGHT)
	{
		j = round(w*cy);
		if (j>0 && i<cy)
		{
			i = x0 + cx;
			j += y0;
			pDC->MoveTo(i,j);
			pDC->LineTo(i-len,j);
		}
	}

	pDC->SelectObject(pOldPen);
}

void CAxis::DrawLabel(CDC *pDC, CRect* rect, const double& w, const char* labelbuf )
// Draw a tick label
// x is actual value or log10(value) if log axis
// val is (x-x0)/(x0-x1)
{
	char test[] = "0";

	int i,j;

	if (type & AXIS_BOTTOM)
	{
		pDC->SetTextAlign(TA_CENTER | TA_TOP);
		i = round(rect[0].Width()*w) + rect[0].left; 
		j = rect[0].top + ticklen;
	}
	else if (type & AXIS_LEFT)
	{
		pDC->SetTextAlign(TA_RIGHT | TA_BOTTOM);
		CSize size = pDC->GetTextExtent(test,1);
		int q = size.cy/2;
		i = rect[1].right - ticklen;
		j = rect[1].top + round(rect[1].Height()*w) + q;
	}
	else if (type & AXIS_TOP)
	{
		pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
		i = rect[2].left + round(rect[2].Width()*w);
		j = rect[2].bottom - ticklen;
	}
	else if (type & AXIS_RIGHT)
	{
		pDC->SetTextAlign(TA_LEFT | TA_BOTTOM);
		CSize size = pDC->GetTextExtent(test,1);
		int q = size.cy/2;
		i = rect[3].left + ticklen;
		j = rect[3].top + round(rect[3].Height()*w) + q;
	}

	pDC->TextOut(i,j,labelbuf,strlen(labelbuf));
}

void CAxis::DrawTicks(CDC *pDC, CRect& rect)
{
	int i;

	if (lin_subticks)
	{
		for (i = min_tick_count-1; i <= max_tick_count; i++) 
		{
			double val = i * tick_spacing;
			for (int j=1; j<lin_subticks; j++)
			{
				val += lin_subtick_spacing;

				if (val < lim[0] || val > lim[1])
				continue;

				double w = val;
				Conv(w);

				DrawTick(pDC,rect,w,true);
			}
		}
	}

	if (have_normal_subsubticks)
    {
		int m, imin, imax;
		double val, range = lim[1] - lim[0];
		
		/* compute an integer range (of powers of 10) large enough to include
		the entire desired axis */
		imin = (int)(floor (lim[0] - FUZZ * range));
		imax = (int)(ceil  (lim[1] + FUZZ * range));
		
		for (i = imin; i < imax; i++)
		{
			for (m = 2; m <= 9 ; m++)
			{
				val = i + log10 ((double)m);
				
				if (val < lim[0] || val > lim[1])
				continue;

				Conv(val);

				DrawTick(pDC,rect,val,true);
			}
		}
    }
	
	for (i = min_tick_count; i <= max_tick_count; i++) 
	{
		double val = i * tick_spacing;
		
		if (val < lim[0] || val > lim[1])
			continue;

		Conv(val);

		DrawTick(pDC,rect,val);
	}


}

void CAxis::DrawLabels(CDC *pDC, CRect *rect)
{
	CGdiObject* pOldFont = pDC->SelectObject(&hfont);
	UINT OldTextAlign = pDC->GetTextAlign();

	int i;
	char labelbuf[128];
	int count = 0;

	for (i = min_tick_count; i <= max_tick_count; i++) 
	{
		double val = i * tick_spacing;
		
		if (val < lim[0] || val > lim[1])
			continue;

		count++;

		PrintTickLabel(labelbuf,val);

		Conv(val);

		DrawLabel(pDC,rect,val,labelbuf);
	}

	if (have_normal_subsubticks && count<2)
	/* log axis with 1 or none labels -> print subtick labels! */
	{
		int m, imin, imax;
		double val, range = lim[1] - lim[0];
		
		/* compute an integer range (of powers of 10) large enough to include
		the entire desired axis */
		imin = (int)(floor (lim[0] - FUZZ * range));
		imax = (int)(ceil  (lim[1] + FUZZ * range));
		
		for (i = imin; i < imax; i++)
		{
			for (m = 2; m <= 9 ; m++)
			{
				val = i + log10 ((double)m);
				
				if (val < lim[0] || val > lim[1])
				continue;

				PrintTickLabel(labelbuf,val);

				Conv(val);

				DrawLabel(pDC,rect,val,labelbuf);
			}
		}
	}
	
	pDC->SelectObject(pOldFont);
	pDC->SetTextAlign(OldTextAlign);

}

void CAxis::DrawTitle(CDC *pDC, CRect* rect)
{
	CGdiObject* pOldFont; 
	UINT OldTextAlign = pDC->GetTextAlign();

	if (type & AXIS_BOTTOM)
	{
		pOldFont = pDC->SelectObject(&hfont);
		pDC->SetTextAlign(TA_TOP | TA_CENTER);
		pDC->TextOut(rect[0].Width()/2+rect[0].left,rect[0].top,Title);
	}
	else if (type & AXIS_LEFT)
	{
		pOldFont = pDC->SelectObject(&vfont);
		pDC->SetTextAlign(TA_BOTTOM | TA_CENTER);
		pDC->TextOut(rect[1].right,rect[1].Height()/2+rect[1].top,Title);
	}
	else if (type & AXIS_TOP)
	{
		pOldFont = pDC->SelectObject(&hfont);
		pDC->SetTextAlign(TA_BOTTOM | TA_CENTER);
		pDC->TextOut(rect[2].Width()/2+rect[2].left,rect[2].bottom,Title);
	}
	else if (type & AXIS_RIGHT)
	{
		pOldFont = pDC->SelectObject(&vfont);
		pDC->SetTextAlign(TA_TOP | TA_CENTER);
		pDC->TextOut(rect[3].left,rect[3].Height()/2+rect[3].top,Title);
	}
	
	pDC->SetTextAlign(OldTextAlign);
	pDC->SelectObject(pOldFont);

}

/* fixing ticks and subticks */

void scale1 (double range, double& tick_spacing, int& tick_spacing_type)
{
	/* valid interval lengths */
	static const double vint[] =
    {
		1.0, 2.0, 5.0, 10.0
    };

	assert(range > 0.);
	
	/* Corresponding breakpoints.  The published algorithm uses geometric
	means, i.e. sqrt(2), sqrt(10), sqrt(50), but using sqrt(10)=3.16...
	will (if nticks=5, as we choose it to be) cause intervals of length
	1.5 to yield an inter-tick distance of 0.2 rather than 0.5.  So we
	could reduce it to 2.95.  Similarly we could reduce sqrt(50) to 6.95
	so that intervals of length 3.5 will yield an inter-tick distance of
	1.0 rather than 0.5. */
	static const double sqr[] =
    {
		1.41421, 3.16228, 7.07107
    };
	
	/* compute trial inter-tick interval length */
	double a = range / TRIAL_NUMBER_OF_TICK_INTERVALS;
	double nal = floor(log10(a));
	double b = a * pow (10.0, -nal);	/* 1.0 <= b < 10.0 */
	
	/* round to closest permissible inter-tick interval length */
	int k = 0;
	do
    {
		if (b < sqr[k])
			break;
		k++;
    }
	while (k < 3);
	
	tick_spacing = vint[k] * pow (10.0, nal);
	/* for increment type, 0,1,2 means 1,2,5 times a power of 10 */
	tick_spacing_type = (k == 3 ? 0 : k);
}

void CAxis::FixDiv()
/* fix the divisions and subdivisions of the axis */
{
	assert(lim[0]<=lim[1]);

	int tick_spacing_type;
	
	if (lim[0] == lim[1])		/* expand in a clever way */
    {
		lim[1] = floor (lim[1] + 1.0);
		lim[0] = ceil (lim[0] - 1.0);
    }
	
	range = lim[1] - lim[0];		/* range is not negative */

	if (base==AXIS_LOG)		/* log axis, data are stored in logarithmic form */
		/* compute a tick spacing; user can't specify it */
    {
		scale1 (range, tick_spacing, tick_spacing_type);
		if (tick_spacing <= 1.0)
		{
			tick_spacing = 1.0;
			tick_spacing_type = S_ONE;
		}
    }
	else				/* linear axis */
    {
		scale1 (range, tick_spacing, tick_spacing_type);
    }


	if (autoscale)	/* expand both limits to next tick */
	{		
		min_tick_count = (int)(floor((lim[0] + FUZZ * range)/ tick_spacing));
		max_tick_count = (int)(ceil((lim[1] - FUZZ * range)/ tick_spacing));
		/* max_tick_count > min_tick_count always */
		/* tickval = tick_spacing * count, 
		for all count in [min_count,max_count]; must have >=2 ticks */
		lim[0] = tick_spacing * min_tick_count;
		lim[1] = tick_spacing * max_tick_count;
		range = lim[1] - lim[0];
	}
	else		/* don't expand limits to next tick */
	{
		min_tick_count = (int)(ceil((lim[0] - FUZZ * range)/ tick_spacing));
		max_tick_count = (int)(floor((lim[1] + FUZZ * range)/ tick_spacing)); 
		/* max_tick_count <= min_tick_count is possible */
		/* tickval = incr * count, 
		for all count in [min_count,max_count]; can have 0,1,2,3... ticks */
	}

	int min_lin_subtick_count,max_lin_subtick_count;
	
	if (base==AXIS_LIN)
	{
	/* Allow 5 subticks per tick if S_FIVE or S_TWO_FIVE, 2 if S_TWO.  Case
		S_ONE is special; we try 10, 5, and 2 in succession */
		
		switch (tick_spacing_type)
		{
		case S_FIVE:
			lin_subtick_spacing = tick_spacing / 5;
			lin_subticks = 5;      
			break;
		case S_TWO:
			lin_subtick_spacing = tick_spacing / 2;
			lin_subticks = 2;
			break;
		case S_ONE:
			lin_subtick_spacing = tick_spacing / 10;
			lin_subticks = 10;
			min_lin_subtick_count = (int)(ceil((lim[0] - FUZZ * range)/ lin_subtick_spacing));
			max_lin_subtick_count = (int)(floor((lim[1] + FUZZ * range)/ lin_subtick_spacing)); 
			if (max_lin_subtick_count - min_lin_subtick_count > MAX_NUM_SUBTICKS)
			{
				lin_subtick_spacing = tick_spacing / 5;
				lin_subticks = 5;
				min_lin_subtick_count = (int)(ceil((lim[0] - FUZZ * range)/ lin_subtick_spacing));
				max_lin_subtick_count = (int)(floor((lim[1] + FUZZ * range)/ lin_subtick_spacing)); 
				if (max_lin_subtick_count - min_lin_subtick_count > MAX_NUM_SUBTICKS)
				{
					lin_subtick_spacing = tick_spacing / 2;
					lin_subticks =  2;
				}
			}
			break;
		default:
			assert(0);
			break;
		}
		
		
		min_lin_subtick_count = (int)(ceil((lim[0] - FUZZ * range)/ lin_subtick_spacing));
		max_lin_subtick_count = (int)(floor((lim[1] + FUZZ * range)/ lin_subtick_spacing)); 
		lin_subticks 
			= (((max_lin_subtick_count - min_lin_subtick_count) <= MAX_NUM_SUBTICKS)
			? lin_subticks : 0);
		
		/* linear axes don't have log subsubticks */
		have_normal_subsubticks = false;
	}
	else /* logarithmic axis */
	{
		if (range <= MAX_DECADES_WITH_LOG_SUBSUBTICKS + FUZZ)
		{
			/* not too many orders of magnitude, so plot normal log subsubticks */
			have_normal_subsubticks = true;
			lin_subticks = 0;
		}
		else
		{
			/* too many orders of magnitude, don't plot log subsubticks */
			have_normal_subsubticks = false;
			
			// check for linear subticks
			// the only possible spacing is 1.0 decade 
			lin_subtick_spacing = 1.0;
			
			min_lin_subtick_count = (int)(ceil((lim[0] - FUZZ * range)/ lin_subtick_spacing));
			max_lin_subtick_count = (int)(floor((lim[1] + FUZZ * range)/ lin_subtick_spacing)); 
			lin_subticks 
				= (((max_lin_subtick_count - min_lin_subtick_count) <= MAX_NUM_SUBTICKS)
				? lin_subticks : 0);
		}
		
	}
	
}

/*void CAxis::SetSize(int asize)
// setting the screen size of the axis
{
	size = asize;
	ax = size/(lim[1] - lim[0]);
}*/

void CAxis::SetLim(double x0, double x1, bool noauto) 
{
	if (noauto) autoscale=false;
	if (autoscale) 
	{
		if (lim[0]==0. && lim[1]==0.) 
		{ 
			lim[0] = x0; lim[1] = x1; 
		}
		else {
			lim[0] = min(lim[0],x0);
			lim[1] = max(lim[1],x1);
		}
	}
	else 
	{ 
		lim[0] = x0; lim[1] = x1; 
	}
	range = (lim[1]-lim[0]);
}

/* popup menu implementation */

void CAxis::AppendMenuItems(CMenu *menu)
{
    if (menu->GetMenuItemCount() != 0)
    {
		//separator
		menu->AppendMenu(MF_SEPARATOR, 0, _T(""));
    };
    UINT menu_flags = MF_STRING | MF_ENABLED;
    if (autoscale)
    {
		menu_flags |= MF_CHECKED;
    };
    menu->AppendMenu(menu_flags, AXIS_TOGGLE_AUTOSCALE, _T("Axis Autoscale"));
    menu->AppendMenu(MF_STRING | MF_ENABLED, GRAPH_ITEM_PROPS, _T("Axis Properties"));
}

bool CAxis::MenuCommand(UINT nIDCommand)
{
	if (CGraphDataItem::MenuCommand(nIDCommand)) return true;
	else {		
		if (nIDCommand==AXIS_TOGGLE_AUTOSCALE) {
			autoscale = !autoscale;
			return true;
		}
		else return false;
	}
}

void CAxis::DoPropDlg()
{
	CAxisDlg dlg(this);
	dlg.DoModal();
}

/* Fonts */

void CAxis::PrepareFonts(int nHeight)
{
	if (hfont.m_hObject!=NULL && vfont.m_hObject!=NULL && nHeight==nFontHeight) return;

	if (nHeight!=nFontHeight || (hfont.m_hObject==NULL && vfont.m_hObject==NULL))
	{
		if (hfont.m_hObject!=NULL) hfont.DeleteObject();
		if (vfont.m_hObject!=NULL) vfont.DeleteObject();

		nFontHeight = nHeight;

		/*hfont.CreateFont(nFontHeight,0,0,0,0,0,0,0,0,
			OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, "Courier New");
		vfont.CreateFont(nFontHeight,0,900,900,0,0,0,0,0,
			OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, "Courier New");*/
		hfont.CreatePointFont(nHeight,"Courier New");
		LOGFONT logfont;
		hfont.GetLogFont(&logfont);
		logfont.lfOrientation = 900;
		logfont.lfEscapement = 900;
		vfont.CreateFontIndirect(&logfont);
	}

}

void CAxis::SetFontHeight(int cx, int cy)
{
/*	int nHeight = 0.05*cy;
	if (nHeight>18) nHeight = 18;
	if (nHeight<10) nHeight = 10;*/
	//int nHeight = 80;
	
	PrepareFonts(nFontHeight);
}


/* Convert data to screen coordinates */

/*int CAxis::Conv(double x, int& ix)
{
	if (base==AXIS_LOG) x = log10(x);
	if (_finite(x)) {
		if ((type & AXIS_BOTTOM) || (type & AXIS_TOP)) ix = (x-lim[0])*ax;
		else ix = (lim[1]-x)*ax;
		return 1;
	} else return 0;
}
int CAxis::Conv(float x, int& ix)
{
	if (base==AXIS_LOG) x = log10(x);
	if (_finite(x)) {
		if ((type & AXIS_BOTTOM) || (type & AXIS_TOP)) ix = (x-lim[0])*ax;
		else ix = (lim[1]-x)*ax;
		return 1;
	} else return 0;
}
int CAxis::Conv(int x, int& ix)
{
	float fx = x;
	if (base==AXIS_LOG) fx = log10(fx);
	if (_finite(fx)) {
		if ((type & AXIS_BOTTOM) || (type & AXIS_TOP)) ix = (fx-lim[0])*ax;
		else ix = (lim[1]-fx)*ax;
		return 1;
	} else return 0;
}
int CAxis::Conv(unsigned int x, int& ix)
{
	float fx = x;
	if (base==AXIS_LOG) fx = log10(fx);
	if (_finite(fx)) {
		if ((type & AXIS_BOTTOM) || (type & AXIS_TOP)) ix = (fx-lim[0])*ax;
		else ix = (lim[1]-fx)*ax;
		return 1;
	} else return 0;
}
int CAxis::Conv(unsigned long x, int& ix)
{
	float fx = x;
	if (base==AXIS_LOG) fx = log10(fx);
	if (_finite(fx)) {
		if ((type & AXIS_BOTTOM) || (type & AXIS_TOP)) ix = (fx-lim[0])*ax;
		else ix = (lim[1]-fx)*ax;
		return 1;
	} else return 0;
}
*/
/* Zoom */ 

void CAxis::Zoom(CRect& old_rect, CRect &new_rect)
{
	if ((type & AXIS_BOTTOM) || (type & AXIS_TOP))
	{
		double ax = old_rect.Width()/range;
		double b = lim[0];
		lim[0] = new_rect.left/ax + b;
		lim[1] = new_rect.right/ax + b;
		autoscale = false;
	}
	else if ((type & AXIS_LEFT) || (type & AXIS_RIGHT))
	{
		double ax = old_rect.Height()/range;
		double b = lim[1];
		lim[0] = b - new_rect.bottom/ax;
		lim[1] = b - new_rect.top/ax;
		autoscale = false;
	}
	range = lim[1]-lim[0];
	//ax = size/(lim[1]-lim[0]);
}

