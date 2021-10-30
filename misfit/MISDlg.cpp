// MISDlg.cpp : implementation file
//

#include "stdafx.h"
#include "misfit.h"
#include "MISDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class  CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMISDlg

IMPLEMENT_DYNAMIC(CMISDlg, CPropertySheet)

CMISDlg::CMISDlg(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage),
	BasicP(IDD_BASIC_PROP,&Data),
	DataP(&Data),
	PlotP(IDD_PLOT_PROP,&Data),
	TrapP(&Data)
{
	AddPage(&BasicP);
	AddPage(&TrapP);
	AddPage(&PlotP);
	AddPage(&DataP);
}

CMISDlg::CMISDlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage),
	BasicP(IDD_BASIC_PROP,&Data),
	DataP(&Data),
	PlotP(IDD_PLOT_PROP,&Data),
	TrapP(&Data)
{
	AddPage(&BasicP);
	AddPage(&TrapP);
	AddPage(&PlotP);
	AddPage(&DataP);
}

CMISDlg::~CMISDlg()
{
	int i=0;
}


BEGIN_MESSAGE_MAP(CMISDlg, CPropertySheet)
	//{{AFX_MSG_MAP(CMISDlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_CALC, OnCalc)
	ON_BN_CLICKED(ID_SAVE_PAR, OnSavePar)
	ON_BN_CLICKED(ID_HTM_HLP, OnHtmHlp)
	ON_BN_CLICKED(ID_ABOUT_BTN, OnAbout)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMISDlg message handlers


BOOL CMISDlg::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	CRect rectWnd;
	GetWindowRect(rectWnd);
	SetWindowPos(NULL, 0, 0,
		rectWnd.Width(),
		rectWnd.Height()+50,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	int nButtons = 4;
	int btW = (rectWnd.Width()-20)/nButtons, btH = 25, top = rectWnd.Height()-15, bottom = top + btH;
	//int left = rectWnd.Width()/2 - 2*(btW+5);
	int left = 10;

	{
		CRect rectPreview(left, top, left + btW, bottom);	
		btCalc.Create("Calc", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			rectPreview, this, ID_CALC);
		btCalc.SetFont(GetFont());
		left += btW;
	}

	/*{
		CRect rectPreview(left, top, left + btW, bottom);	
		btFit.Create("Fit", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			rectPreview, this, ID_FIT);
		btFit.SetFont(GetFont());
		left += btW;
		btFit.EnableWindow(FALSE);
	}*/

	{
		CRect rectPreview(left, top, left + btW, bottom);	
		btSaveParams.Create("SavePar", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			rectPreview, this, ID_SAVE_PAR);
		btSaveParams.SetFont(GetFont());
		left += btW;
	}

	{
		CRect rectPreview(left, top, left + btW, bottom);	
		btHelp.Create("Help", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			rectPreview, this, ID_HTM_HLP);
		btHelp.SetFont(GetFont());
		left += btW;
	}

	{
		CRect rectPreview(left, top, left + btW, bottom);	
		btAbout.Create("About", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			rectPreview, this, ID_ABOUT_BTN);
		btAbout.SetFont(GetFont());
		left += btW;
	}

	Data.UpdatePlot();

	return bResult;
}

void CMISDlg::OnSavePar() 
{
	PressButton(PSBTN_APPLYNOW);   

	static char BASED_CODE szFilter[] = 
		"Data files (*.dat)|*.dat|Text files (*.txt)|*.txt|All Files (*.*)|*.*||";

	CFileDialog dlg(FALSE,".dat",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);

	if (!(dlg.DoModal()==IDOK)) return;

	Data.SavePar(dlg.GetPathName());
}

void CMISDlg::OnReplot()
{
	PressButton(PSBTN_APPLYNOW);   
	Data.UpdatePlot();
}

void CMISDlg::OnCalc()
{
	PressButton(PSBTN_APPLYNOW);   
	Data.DoCalc();
	BasicP.UpdateData(0);
}

void CMISDlg::OnHtmHlp()
{
	CWinApp* pApp = AfxGetApp();
	CString path(pApp->m_pszHelpFilePath);
	//CString file(::PathFindFileName(pApp->m_pszHelpFilePath));
	path.MakeUpper();
	int i = path.Find("MISFIT.HLP");
	path.Delete(i,strlen("MISFIT.HLP"));
	path += "misfit.html";

	//MessageBox(path);


	ShellExecute(this->m_hWnd,
		NULL,
		path,
		NULL,
		NULL,
		0
		);
}

void CMISDlg::OnAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CMISDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	EndModalLoop(0);
	
	//CPropertySheet::OnClose();
}
