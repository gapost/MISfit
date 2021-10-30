// TrapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "misfit.h"
#include "TrapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrapDlg dialog


CTrapDlg::CTrapDlg(CTrapData* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CTrapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrapDlg)
	//}}AFX_DATA_INIT
	ASSERT(pData!=NULL);
	pTrapData = pData;
	TempData = *pData;
}


void CTrapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrapDlg)
	//}}AFX_DATA_MAP
	DDX_CBIndex(pDX, IDC_TRAP_MODE, TempData.nTrapMode);
	DDX_Text(pDX, IDC_DIT, TempData.Dit);
	DDX_Text(pDX, IDC_SS, TempData.sigma);
	DDX_Radio(pDX, IDC_TRAPTYPE, TempData.nTraptype);
	DDX_Text(pDX, IDC_E0, TempData.Eo);
	DDX_Text(pDX, IDC_DE, TempData.DE);

	UINT i,k=0;
	for (i=IDC_DITM; i<=IDC_DITM5; i++) {
		DDX_Text(pDX, i, TempData.Dit_[k++]);
	}
	k=0;
	for (i=IDC_ET; i<=IDC_ET5; i++) {
		DDX_Text(pDX, i, TempData.E_[k++]);
	}
}


BEGIN_MESSAGE_MAP(CTrapDlg, CDialog)
	//{{AFX_MSG_MAP(CTrapDlg)
	ON_CBN_SELCHANGE(IDC_TRAP_MODE, OnChangeTrapMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrapDlg message handlers

void CTrapDlg::OnOK() 
{
	UpdateData(TRUE);
	TempData.SortArrays();
	*pTrapData = TempData;
	
	CDialog::OnOK();
}

BOOL CTrapDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTrapDlg::EnableCtrls()
{
	UpdateData(TRUE);

	BOOL f = (TempData.nTrapMode<3);
	GetDlgItem(IDC_DIT)->EnableWindow(f);

	f = (TempData.nTrapMode==1 || TempData.nTrapMode==2);
	GetDlgItem(IDC_E0)->EnableWindow(f);
	GetDlgItem(IDC_DE)->EnableWindow(f);

	f = (TempData.nTrapMode==3);
	UINT i,k=0;
	for (i=IDC_DITM; i<=IDC_DITM5; i++) GetDlgItem(i)->EnableWindow(f);
	for (i=IDC_ET; i<=IDC_ET5; i++) GetDlgItem(i)->EnableWindow(f);
}

void CTrapDlg::OnChangeTrapMode() 
{
	EnableCtrls();	
}
