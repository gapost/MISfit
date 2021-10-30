/*
This file was created by Paul Barvinko (pbarvinko@yahoo.com).
This file is distributed "as is", e.g. there are no warranties 
and obligations and you could use it in your applications on your
own risk. 
Your comments and questions are welcome.
If using in your applications, please mention author in credits for your app.
*/
#include "stdafx.h"
#include "offscreen.h"

COffScreen::COffScreen() : m_pMemDC(NULL), m_pOldBitmap(NULL), H(0), W(0)
{
}

COffScreen::~COffScreen()
{
	Destroy();
}

void COffScreen::Destroy()
{
	if (m_pMemDC != NULL)
	{
		m_pMemDC->SelectObject(m_pOldBitmap);
		delete m_pMemDC;
		m_pMemDC = NULL;
		m_pOldBitmap = NULL;
	}

	if (m_Bitmap.m_hObject != NULL) {
		m_Bitmap.DeleteObject();
	}
}

void COffScreen::Create(int cx, int cy, BOOL bErase, COLORREF clr )
{
	W = cx;
	H = cy;
	Create(bErase,clr);
}

void COffScreen::Create(BOOL bErase, COLORREF clr )
{
	Destroy();
	
	m_pMemDC = new CDC;

	CDC* pDC = CDC::FromHandle(::GetDC(NULL));
	
	VERIFY( m_pMemDC->CreateCompatibleDC(pDC) );

	VERIFY( m_Bitmap.CreateCompatibleBitmap(pDC, W, H) );

	m_pOldBitmap = m_pMemDC->SelectObject( &m_Bitmap );
	
	if (bErase) m_pMemDC->FillSolidRect(0,0,W,H,clr);
}

void COffScreen::Resize(int cx, int cy, BOOL bErase, COLORREF clr )
{
	if (cx!=W || cy!=H)
	{
		W = cx;
		H = cy;
		Create(bErase);
	} else
	{
		if (bErase) m_pMemDC->FillSolidRect(0,0,W,H,clr);
	};
}

void COffScreen::DrawHere(CDC* pDC, const CRect& rect)
{
	if (m_Bitmap.m_hObject != NULL)
	{
		pDC->BitBlt(
			rect.left,
			rect.top,
			rect.Width(),
			rect.Height(),
			m_pMemDC, 
			rect.left,
			rect.top, 
			SRCCOPY 
			);
	}
}


void COffScreen::DrawHere(CDC *pDC, const CRect &rect, const CPoint &offset)
{
	if (m_Bitmap.m_hObject != NULL)
	{
		pDC->BitBlt(
			offset.x + rect.left,
			offset.y + rect.top,
			rect.Width(),
			rect.Height(),
			m_pMemDC, 
			rect.left,
			rect.top, 
			SRCCOPY 
			);
	}
}
