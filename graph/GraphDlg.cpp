// GraphDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GraphWnd.h"
#include "GraphDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxisDlg dialog


CAxisDlg::CAxisDlg(CAxis* pAxis, CWnd* pParent /*=NULL*/)
	: CDialog(CAxisDlg::IDD, pParent),
	m_pAxis(pAxis)
{
	//{{AFX_DATA_INIT(CAxisDlg)
	//}}AFX_DATA_INIT
	bAutoscale = m_pAxis->IsAuto();
	nAxisBase = m_pAxis->GetBase();
	strTitle = m_pAxis->Title;

	double v = m_pAxis->lim[0];
	m_pAxis->InvBaseTransform(v);
	x0 = v;

	v = m_pAxis->lim[1];
	m_pAxis->InvBaseTransform(v);
	x1 = v;
}

void CAxisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxisDlg)
	DDX_Control(pDX, IDC_EDIT_X1, m_edtX1);
	DDX_Control(pDX, IDC_EDIT_X0, m_edtX0);
	DDX_Check(pDX, IDC_CHECK_AUTOSCALE, bAutoscale);
	DDX_CBIndex(pDX, IDC_BASE_COMBO, nAxisBase);
	DDX_Text(pDX, IDC_EDIT_TITLE, strTitle);
	DDX_Text(pDX, IDC_EDIT_X0, x0);
	DDX_Text(pDX, IDC_EDIT_X1, x1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAxisDlg, CDialog)
	//{{AFX_MSG_MAP(CAxisDlg)
	ON_BN_CLICKED(IDC_CHECK_AUTOSCALE, OnAutoscale)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisDlg message handlers

void CAxisDlg::OnAutoscale() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_edtX1.EnableWindow(!bAutoscale);
	m_edtX0.EnableWindow(!bAutoscale);	
}

void CAxisDlg::OnOK() 
{
	UpdateData(TRUE);
	m_pAxis->SetAuto(bAutoscale);
	m_pAxis->SetLog(nAxisBase==AXIS_LOG);
	m_pAxis->Title = strTitle;
	if(!bAutoscale)
	{
		double dx0 = x0, dx1 = x1;
		if (m_pAxis->BaseTransform(dx0) && m_pAxis->BaseTransform(dx1))
			m_pAxis->SetLim(dx0,dx1);
		else m_pAxis->SetAuto();
	}
	CDialog::OnOK();
}

BOOL CAxisDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_edtX1.EnableWindow(!bAutoscale);
	m_edtX0.EnableWindow(!bAutoscale);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
// CSeriesDlg dialog


CSeriesDlg::CSeriesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSeriesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeriesDlg)
	//}}AFX_DATA_INIT
}


void CSeriesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeriesDlg)
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_RADIO1, m_ntype);
}

void CSeriesDlg::SetType(int atype)
{
	if (atype & SERIES_HIST) m_ntype = 3;
	else if ((atype & SERIES_LINE) && (atype & SERIES_SCATTER)) m_ntype = 2;
	else if (atype & SERIES_LINE) m_ntype = 1;
	else if (atype & SERIES_SCATTER) m_ntype = 0;
}

int CSeriesDlg::GetType()
{
	switch (m_ntype) {
	case 0: return SERIES_SCATTER; break;
	case 1: return SERIES_LINE; break;
	case 2: return SERIES_SCATTER | SERIES_LINE; break;
	case 3: return SERIES_HIST; break;
	}
	return SERIES_LINE;
}



BEGIN_MESSAGE_MAP(CSeriesDlg, CDialog)
	//{{AFX_MSG_MAP(CSeriesDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeriesDlg message handlers


