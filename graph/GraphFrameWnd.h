#if !defined(AFX_GRAPHFRAMEWND_H__75561CAC_6CFB_4511_933C_38D81A94C39B__INCLUDED_)
#define AFX_GRAPHFRAMEWND_H__75561CAC_6CFB_4511_933C_38D81A94C39B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphFrameWnd.h : header file
//

class CGraphWnd;

class AFX_EXT_CLASS CGraphFrame
{
public:
	CGraphFrame(const CString& S = "", int nRow = 1, int nCol = 1, int W = 600, int H = 400);
	~CGraphFrame();

	CGraphWnd* GetGraphWnd();

	void Pause();

protected:
	CWinThread* m_pThread;
};

/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHFRAMEWND_H__75561CAC_6CFB_4511_933C_38D81A94C39B__INCLUDED_)
