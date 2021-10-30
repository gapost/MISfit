/*
This file was created by Paul Barvinko (pbarvinko@yahoo.com).
This file is distributed "as is", e.g. there are no warranties 
and obligations and you could use it in your applications on your
own risk. 
Your comments and questions are welcome.
If using in your applications, please mention author in credits for your app.
*/
#ifndef _F2DOFF_H_
#define _F2DOFF_H_

class COffScreen
{
public:
	void DrawHere(CDC* pDC, const CRect& rect, const CPoint& offest);
	COffScreen();
	~COffScreen();

	void Create(int cx, int cy, BOOL bErase=TRUE, COLORREF clr = RGB(255,255,255) );

	// access to members
	int Width() { return W; }
	int Height() { return H; }

	// DC from offscreens
	HDC GetHDC()
	{
		if (m_pMemDC) return m_pMemDC->m_hDC;
		else return 0;
	}
	CDC* GetDC() { return m_pMemDC; }

	//Draw capabilities
	//void DrawDib(HDC dc, CRect r);
	void DrawHere(CDC* pDC, const CRect& rect);
	//void SetMemoryPoint(int x, int y, BYTE value);
	//void DrawCross(int x, int y, int size, BYTE color);
	
	// resize options
	void Resize(int cx, int cy, BOOL bErase = TRUE, COLORREF clr = RGB(255,255,255));
	
protected:
	// Offscreen DC
	CDC *m_pMemDC;

	// Bitmap Attributes
	CBitmap m_Bitmap;
	CBitmap* m_pOldBitmap;

	int W,H;
	
	void Create(BOOL bErase = TRUE, COLORREF clr = RGB(255,255,255));
	void Destroy();
};

#endif
