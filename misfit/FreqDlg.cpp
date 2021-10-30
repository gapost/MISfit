// FreqDlg.cpp : implementation file
//

#include "stdafx.h"
#include "misfit.h"
#include "FreqDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFreqDlg dialog


CFreqDlg::CFreqDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFreqDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFreqDlg)
	Freq = 1000.0f;
	//}}AFX_DATA_INIT
}


void CFreqDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFreqDlg)
	DDX_Text(pDX, IDC_EDIT1, Freq);
	DDV_MinMaxFloat(pDX, Freq, 1.f, 1.e+007f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFreqDlg, CDialog)
	//{{AFX_MSG_MAP(CFreqDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFreqDlg message handlers

BOOL CFreqDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
