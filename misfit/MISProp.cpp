// MISProp.cpp : implementation file
//

#include "stdafx.h"
#include "misfit.h"
#include "MISProp.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMISProp property page

IMPLEMENT_DYNCREATE(CMISProp, CPropertyPage)

CMISProp::CMISProp(UINT uID, CMISData* aMISData, BOOL isExec) : 
CPropertyPage(uID), pMISData(aMISData), bExecPage(isExec)
{
	//{{AFX_DATA_INIT(CMISProp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMISProp::~CMISProp()
{
}

void CMISProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMISProp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	pMISData->DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMISProp, CPropertyPage)
	//{{AFX_MSG_MAP(CMISProp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMISProp message handlers

void CMISData::DoDataExchange(CDataExchange *pDX)
{
	if (!pDX->m_bSaveAndValidate) EOT = 3.9*VACUUM_KAPPA/Cox*1e6/L0;
	
	CWnd* pWnd = pDX->m_pDlgWnd;
	if (pWnd->GetDlgItem(IDC_COX))
	DDX_Text(pDX, IDC_COX, Cox);
	if (pWnd->GetDlgItem(IDC_DOPING))
	DDX_Text(pDX, IDC_DOPING, Nd);
	DDV_MinMaxFloat(pDX, Nd, 1e13f, 1e18f);

	//if (pWnd->GetDlgItem(IDC_EOT))
	//DDX_Text(pDX, IDC_EOT, EOT);

	if (pWnd->GetDlgItem(IDC_QUANT))
	DDX_Check(pDX, IDC_QUANT, bQuant);
	if (pWnd->GetDlgItem(IDC_SUBTYPE))

	DDX_Radio(pDX, IDC_SUBTYPE, nSubtype);

	if (pWnd->GetDlgItem(IDC_COMBO_SUB))
	DDX_CBIndex(pDX, IDC_COMBO_SUB, nSubMaterial);

	if (pWnd->GetDlgItem(IDC_VFB))
	DDX_Text(pDX, IDC_VFB, Vfb);
	if (pWnd->GetDlgItem(IDC_GP))
	DDX_Text(pDX, IDC_GP, Gp);
	if (pWnd->GetDlgItem(IDC_GS))
	DDX_Text(pDX, IDC_GS, Gs);
	if (pWnd->GetDlgItem(IDC_PLOTX_COMBO))
	DDX_CBIndex(pDX, IDC_PLOTX_COMBO, PlotOptions.typex);
	if (pWnd->GetDlgItem(IDC_PLOTY_COMBO))
	DDX_CBIndex(pDX, IDC_PLOTY_COMBO, PlotOptions.typey);

	//if (pWnd->GetDlgItem(IDC_TRAP_SHIFT))
	//DDX_Text(pDX, IDC_TRAP_SHIFT, Vts);

	if (pWnd->GetDlgItem(IDC_CORR))
	DDX_Check(pDX, IDC_CORR, PlotOptions.GsCorrect);


	if (pDX->m_bSaveAndValidate)
	{
		CWnd *pEdt = pWnd->GetDlgItem(IDC_EOT);
		if (pEdt) {
			EOT = 3.9*VACUUM_KAPPA/Cox*1e6/L0;
			CString S;
			S.Format("%.3g",EOT);
			pEdt->SetWindowText(S);
		}
	}
	else
	{
		CWnd *pEdt = pWnd->GetDlgItem(IDC_TRAP_SHIFT);
		if (pEdt) {
			CString S;
			S.Format("%.3g",Vts);
			pEdt->SetWindowText(S);
		}
	}

	PlotOptions.logG = (PlotOptions.typey>0);
	PlotOptions.logC = (PlotOptions.typey==2);
}

void CMISProp::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::OnOK();

	if (bExecPage) pMISData->DoCalc();
}

void CMISData::DoCalc()
{
	if (IsEmpty()) return;

	CWaitCursor WC;

	CQitArray Qit;
	int nt = Traps.GetSize();
	for(int i=0; i<nt; i++) Traps[i].AddTrap(Qit,nSubtype==0,nSubMaterial);

	int mistype;
	if (bQuant && nSubtype==1) mistype = TYPE_PQ;
	if (bQuant && nSubtype==0) mistype = TYPE_NQ;
	if (!bQuant && nSubtype==1) mistype = TYPE_PCL;
	if (!bQuant && nSubtype==0) mistype = TYPE_NCL;

	CMISitInterpBase* pMIS;

	switch(nSubMaterial)
	{
	case 0:
		pMIS = new CMISitInterp(&Qit, mistype, Cox*1e-6, (float)log10(Nd), Vfb);
		break;
	case 1:
		pMIS = new CGeMISitInterp(&Qit, mistype, Cox*1e-6, (float)log10(Nd), Vfb);
		break;
	case 2:
		pMIS = new CGaAsMISitInterp(&Qit, mistype, Cox*1e-6, (float)log10(Nd), Vfb);
		break;
	}
	

	Vts = -pMIS->GetTrapShift();

	CalcGV(pMIS,Gs,Gp,PlotOptions);
	Plot(Graph.GetGraphWnd(),PlotOptions);	
}

BOOL CMISProp::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CPropertyPage::SetModified(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMISData::UpdatePlot()
{
	Graph.GetGraphWnd()->Clear();
	Plot(Graph.GetGraphWnd(),PlotOptions);	
}

void CMISData::SavePar(CString &S)
{
	ofstream os(S);

	os << "QitFit - MIS Simulation output:\n\nModel parameters:\n\n";

	os << "Si substr.  = ";
	if (nSubtype==1) os << "p-type"; else os << "n-type";
	os << " " << Nd << " (#/cm3)\n\n";


	os << "EOT (nm)     = " << EOT << endl;
	os << "Cox (uF/cm2) = " << Cox << endl;
	os << "Vfb (Volt)   = " << Vfb << endl;
	os << "Trap Shift (Volt)   = " << Vts << endl;
	os << "QM corr.     = ";
	if (bQuant) os << "Yes\n"; else os << "No\n";

	os << endl;

	os << "Interface Traps  : \n";
	int ntrap = Traps.GetSize();
	for(int i=0; i<ntrap; i++) {
		os << "   " << "Trap " << i+1 << endl;
		Traps[i].dump(os);
		os << endl;
	}

	os << "Parasitic Conductances  : \n";
	os << "Gs (S/cm^2) = " << Gs << endl;
	os << "Gp (S/cm^2) = " << Gp << endl;

	os.close();	
}
