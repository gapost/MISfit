// GraphFrameWnd.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "GraphFrameWnd.h"

#include "GraphWnd.h"

#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphFrameWnd frame

class CGraphFrameWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CGraphFrameWnd)
public:
	CGraphFrameWnd(int aRow = 1, int aCol = 1, int aW = 600, int aH = 600);     // protected constructor used by dynamic creation

// Attributes
public:
	CGraphWnd m_wndGraph;
	BOOL m_bAllowClose;
	int nRow, nCol, W, H;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphFrameWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGraphFrameWnd();

	// Generated message map functions
	//{{AFX_MSG(CGraphFrameWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CGraphThread thread

class CGraphThread : public CWinThread
{
	DECLARE_DYNCREATE(CGraphThread)
protected:

// Attributes
public:
	CGraphThread(int aRow = 1, int aCol = 1, int aW = 600, int aH = 400);    

	int nRow, nCol, W, H;

	CGraphFrameWnd *m_pFrame;

	CEvent FrameCreated;

	CGraphFrameWnd* GetFrameWnd() { return m_pFrame; }


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGraphThread();

	// Generated message map functions
	//{{AFX_MSG(CGraphThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


CGraphFrame::CGraphFrame(const CString& S, int nRow, int nCol, int W, int H)
{
	m_pThread = new CGraphThread(nRow,nCol, W, H);
	if (m_pThread!=NULL) {

		m_pThread->CreateThread();

		WaitForSingleObject(
			((CGraphThread*)m_pThread)->FrameCreated.m_hObject,
			INFINITE
			);

		((CGraphThread*)m_pThread)->GetFrameWnd()->SetWindowText(S);
	}
}

CGraphFrame::~CGraphFrame()
{
	if (m_pThread!=NULL) {

		CGraphFrameWnd* pFrame = ((CGraphThread*)m_pThread)->GetFrameWnd();
		pFrame->m_bAllowClose = TRUE;

		pFrame->PostMessage(WM_CLOSE);
		PostThreadMessage(m_pThread->m_nThreadID,WM_QUIT,0,0);

		WaitForSingleObject(m_pThread->m_hThread,INFINITE);
	}
}

CGraphWnd* CGraphFrame::GetGraphWnd()
{
	if (m_pThread!=NULL) 
		return &(((CGraphThread*)m_pThread)->GetFrameWnd()->m_wndGraph);
	else return NULL;

}

void CGraphFrame::Pause()
{
	GetGraphWnd()->MessageBox("Continue?",NULL);
}




/////////////////////////////////////////////////////////////////////////////
// CGraphFrameWnd

IMPLEMENT_DYNCREATE(CGraphFrameWnd, CFrameWnd)

CGraphFrameWnd::CGraphFrameWnd(int aRow, int aCol, int aW, int aH)
{
	nRow = aRow;
	nCol = aCol;
	W = aW;
	H = aH;
}

CGraphFrameWnd::~CGraphFrameWnd()
{
}


BEGIN_MESSAGE_MAP(CGraphFrameWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CGraphFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphFrameWnd message handlers

int CGraphFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetIcon(AfxGetApp()->LoadIcon(IDI_GRAPH),TRUE);
	
	CRect rect;
	GetClientRect(rect);
	m_wndGraph.Create(rect, this, nRow, nCol);
	
	return 0;
}

void CGraphFrameWnd::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	m_wndGraph.MoveWindow(0, 0, cx, cy);
	
}

/////////////////////////////////////////////////////////////////////////////
// CGraphThread

IMPLEMENT_DYNCREATE(CGraphThread, CWinThread)

CGraphThread::CGraphThread(int aRow, int aCol, int aW, int aH) : FrameCreated(FALSE, TRUE)
{
	nRow = aRow;
	nCol = aCol;
	W = aW;
	H = aH;
	m_pFrame = NULL;
}

CGraphThread::~CGraphThread()
{
}

BOOL CGraphThread::InitInstance()
{
	m_pFrame = new CGraphFrameWnd(nRow, nCol, W, H);

	m_pFrame->Create(NULL,"Graph",WS_OVERLAPPEDWINDOW | WS_VISIBLE);

	m_pFrame->m_bAllowClose = FALSE;

	m_pMainWnd = m_pFrame;

	FrameCreated.SetEvent();

	return TRUE;
}

int CGraphThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	//m_pMainWnd->DestroyWindow();

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CGraphThread, CWinThread)
	//{{AFX_MSG_MAP(CGraphThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphThread message handlers

BOOL CGraphFrameWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.cy = H;
	cs.cx = W;
	
	return CFrameWnd::PreCreateWindow(cs);
}

void CGraphFrameWnd::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bAllowClose) CFrameWnd::OnClose();
	else 
		MessageBox(
		"You cannot close this window!\nFisrt terminate the host application.",
		NULL,
		MB_ICONWARNING | MB_OK);
}
