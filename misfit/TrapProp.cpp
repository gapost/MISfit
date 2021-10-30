// TrapProp.cpp : implementation file
//

#include "stdafx.h"
#include "misfit.h"
#include "TrapProp.h"
#include "trapdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrapProp property page

IMPLEMENT_DYNCREATE(CTrapProp, CMISProp)

CTrapProp::CTrapProp(CMISData* aMISData) 
: CMISProp(CTrapProp::IDD,aMISData)
{
	//{{AFX_DATA_INIT(CTrapProp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CTrapProp::~CTrapProp()
{
}

void CTrapProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrapProp)
	DDX_Control(pDX, IDC_TRAP_LIST, lbTraps);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrapProp, CMISProp)
	//{{AFX_MSG_MAP(CTrapProp)
	ON_BN_CLICKED(IDC_ADD_TRAP, OnAddTrap)
	ON_BN_CLICKED(IDC_DEL_TRAP, OnDelTrap)
	ON_BN_CLICKED(IDC_EDIT_TRAP, OnEditTrap)
	ON_LBN_DBLCLK(IDC_TRAP_LIST, OnDblclkTrapList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrapProp message handlers

void CTrapProp::OnAddTrap() 
{
	CTrapData td(pMISData->eGap());
	CTrapDlg dlg(&td);
	if (dlg.DoModal()==IDOK) {
		int i = pMISData->Traps.Add(td);
		lbTraps.AddString(td.MakeTitle());
		lbTraps.SetCurSel(i);
	}	
}

void CTrapProp::OnDelTrap() 
{
	int i = lbTraps.GetCurSel();
	if (i!=LB_ERR) {
		pMISData->Traps.RemoveAt(i);
		lbTraps.DeleteString(i);
	}
	
}

void CTrapProp::OnEditTrap() 
{
	int i = lbTraps.GetCurSel();
	if (i!=LB_ERR) {
		CTrapDlg dlg(&(pMISData->Traps[i]));
		if (dlg.DoModal()==IDOK) {
			lbTraps.DeleteString(i);
			lbTraps.InsertString(i,pMISData->Traps[i].MakeTitle());
			lbTraps.SetCurSel(i);
		}
	}	
}

void CTrapProp::OnDblclkTrapList() 
{
	OnEditTrap();	
}
